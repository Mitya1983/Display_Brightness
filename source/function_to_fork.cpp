#include "function_to_fork.h"
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <memory>
#include <array>
#include <log.h>
#include <fstream>
#include <thread>
#include <iostream>
#include <sys/inotify.h>
#include "display.h"
#include "image_from_camera.h"
#include <constant_names.h>
//Global variable for signal handling
bool working;
bool read_new_brightness_values;
std::condition_variable trigger_for_main_thread;
std::mutex mtx;

void MT::brightness_adjust(Config config)
{
    //Setting up the system signal handling
    struct sigaction signal_action;
    signal_action.sa_handler = &display_brightness_signal_handler;
    if (sigaction(SIGQUIT, &signal_action, nullptr) == -1){
        std::string msg = "Error on catching SIGQUIT: ";
        msg += std::error_code(errno, std::generic_category()).message();
        MT::Log::log().writeToLog(msg);
        MT::Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGINT, &signal_action, nullptr) == -1){
        std::string msg = "Error on catching SIGINT: ";
        msg += std::error_code(errno, std::generic_category()).message();
        MT::Log::log().writeToLog(msg);
        MT::Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGTERM, &signal_action, nullptr) == -1){
        std::string msg = "Error on catching SIGTERM: ";
        msg += std::error_code(errno, std::generic_category()).message();
        MT::Log::log().writeToLog(msg);
        MT::Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGUSR1, &signal_action, nullptr) == -1){
        std::string msg = "Error on catching SIGUSR1: ";
        msg += std::error_code(errno, std::generic_category()).message();
        MT::Log::log().writeToLog(msg);
        MT::Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }
    /******************************************************************************************************************/
    //VARIABLES

    //Variable to handle display brightness
    std::unique_ptr<Display> display = std::make_unique<Display>();
    display->set_max_brightness_file(config.max_brightness_file());
    display->set_cur_brightness_file(config.display_brightness_file());
    display->set_state_file(config.display_state_file());
    //SET DISPLAY PARAPMS FROM CONFIG

    //Setting up array of brightness values which correspond to luminocity (grayscale) level
    std::array<short, 256> brightness_values;
    setup_brightness_values(brightness_values, display->max_brightness());

    //Variable to handle image capture
    std::unique_ptr<Image> image = std::make_unique<Image>();

    //bool variable to check if capture is made for first time since application start
    //was noticed that for first time captured image is to dark, thus display brightness is set to low
    bool first_image = true;

    //Variable to store image capturing time interval in seconds
    //By default is 30 seconds
    unsigned int default_time_interval = config.time_interval();
    unsigned int time_interval = default_time_interval;

    //Variable to store the count of unchanged luminocity checks
    //Used to increase time_imterval variable
    int times_brightness_wasnt_changed = 0;

    //Variables to pass signal to notifier thread to stop
    std::promise<void> signal_to_stop;
    std::future<void> signal_send = signal_to_stop.get_future();

    //bool variable to store whether brightness was set by user
    bool brightness_was_changed_by_user = false;

    //bool variable to store whether brightness was set by application
    bool brightness_was_changed_by_app = false;

    //Writing status to file
    try {
        MT::write_status_to_file(MT::Status::Running);
    } catch (std::fstream::failure &e) {
        std::cout << e.what() << std::endl;
        Log::log().writeToLog(e.what());
        Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }
    //Writing pid to file
    try {
        MT::write_pid_to_file();
    } catch (std::fstream::failure &e) {
        std::cout << e.what() << std::endl;
        Log::log().writeToLog(e.what());
        Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }
    working = true;
    //Lunching notifier thread
    std::thread notifier(MT::brightness_change_notifier, std::ref(trigger_for_main_thread),
                         std::ref(brightness_was_changed_by_user), std::ref(brightness_was_changed_by_app),
                         std::move(signal_send), std::ref(config.actual_display_brightness_file()));

    //Main loop
    MT::Log::log().writeToLog("Starting luminocity check");
    while (true){
        if (!brightness_was_changed_by_user && display->is_on()){
            try {
                image->capture_frame();
                if (first_image){//Second frame for first invokation (noticed that first camera shot is not accurate)
                    image->capture_frame();
                    first_image = false;
                }
            } catch (std::runtime_error &e) {
                MT::Log::log().writeToLog(e.what());
                MT::write_status_to_file(MT::Status::Stopped);
                signal_to_stop.set_value();
                notifier.join();
                MT::Log::log().writeToLog("Exiting with failure");
                exit(EXIT_FAILURE);
            }
            try {
                bool brightness_was_changed = display->set_cur_brightness(brightness_values[image->average_grayscale_value()]);
                if (brightness_was_changed){
                    times_brightness_wasnt_changed = 0;
                    brightness_was_changed_by_app = true;
                }
                else {
                    ++times_brightness_wasnt_changed;
                    if (times_brightness_wasnt_changed == 2){
                        std::cout << time_interval << std::endl;
                        time_interval *= 2;
                    }
                    else if (times_brightness_wasnt_changed == 4){
                        std::cout << time_interval << std::endl;
                        time_interval *= 2;
                    }
                    else if (times_brightness_wasnt_changed == 8){
                        std::cout << time_interval << std::endl;
                        time_interval *= 2;
                    }
                    else if (times_brightness_wasnt_changed == 16){
                        std::cout << time_interval << std::endl;
                        time_interval *= 2;
                    }
                }
            } catch (std::fstream::failure &e) {
                MT::Log::log().writeToLog(e.what());
                MT::write_status_to_file(MT::Status::Stopped);
                signal_to_stop.set_value();
                notifier.join();
                MT::Log::log().writeToLog("Exiting with failure");
                exit(EXIT_FAILURE);
            }
        }
        else{
            if (brightness_was_changed_by_user){
                mtx.lock();
                brightness_was_changed_by_user = false;
                mtx.unlock();
            }
            mtx.unlock();
        }
        if (!working){
            break;
        }
        std::unique_lock<std::mutex> lock(mtx);
        trigger_for_main_thread.wait_for(lock, std::chrono::seconds(time_interval));
        if (!working){
            break;
        }
        if (read_new_brightness_values){
            MT::setup_brightness_values(brightness_values, display->max_brightness());
            read_new_brightness_values = false;
        }
        if (time_interval == 9000){
            time_interval = default_time_interval;
            MT::write_status_to_file(MT::Status::Running);
        }
        if (brightness_was_changed_by_user){
            display->set_cur_brightness_from_file();
            time_interval = 9000;
            MT::write_status_to_file(MT::Status::Sleep);
        }

    }
    MT::write_status_to_file(MT::Status::Stopped);
    signal_to_stop.set_value();
    notifier.join();
    Log::log().writeToLog("Exiting luminocity check");
}
//Notifier function
void MT::brightness_change_notifier(std::condition_variable &cv, bool &signal_to_read_brightness,
                                    bool &brightness_was_set_by_app, std::future<void> signal_to_stop,
                                    const std::string &actual_brightness_file)
{
    //READ actual_brightness_file_path from file
    MT::Log::log().writeToLog("Starting notify thread");
    int notifier = inotify_init1(IN_NONBLOCK);
    //Add notifier check here
    int file_to_watch = inotify_add_watch(notifier, actual_brightness_file.c_str(), IN_MODIFY);
    if (file_to_watch < 0){
        std::string msg = "void MT::brightness_change_notifier(std::condition_variable &cv, bool &signal_to_read_brightness, std::future<void> signal_to_stop): ";
        msg += std::error_code(errno, std::generic_category()).message();
        MT::Log::log().writeToLog(msg);
        MT::Log::log().writeToLog("Exiting notify thread with failure");
        return;
    }
    std::array<char, 64> buffer;
    while (signal_to_stop.wait_for(std::chrono::milliseconds(500))==std::future_status::timeout){
        int status = read(notifier, buffer.data(), 64);
        if (status >= 0){
            if (brightness_was_set_by_app){
                brightness_was_set_by_app = false;
            }
            else{
                std::mutex mtx;
                std::unique_lock<std::mutex> lock(mtx);
                signal_to_read_brightness = true;
                cv.notify_one();
            }
        }
    }
    close(notifier);
    MT::Log::log().writeToLog("Exiting notify thread");
}

void MT::write_pid_to_file()
{
    std::ofstream file(MT::Constants::pid_file_name, std::ios_base::trunc);
    if(!file.is_open()){
        std::string msg = "void MT::write_pid_to_file(): ERROR opening \'pid\' file for writing - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    int pid = static_cast<int>(getpid());
    file << pid << std::endl;
    file.close();
}

pid_t MT::read_pid_from_file()
{
    std::ifstream file(MT::Constants::pid_file_name);
    if(!file.is_open()){
        std::string msg = "pid_t MT::read_pid_from_file(): ERROR opening \'pid\' file for reading - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    std::string value;
    std::getline(file, value);
    pid_t pid = std::stoi(value);
    return pid;
}

void MT::write_status_to_file(Status status)
{
    std::ofstream status_file(MT::Constants::status_file_name, std::ios_base::trunc);
    if (!status_file.is_open()){
        std::string msg = "void write_process_id_to_file(int process_id): ERROR on opening \'status\' file for writing - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    std::string status_str;
    switch (status) {
    case MT::Status::Running:{
        status_str = "running";
        break;
    }
    case MT::Status::Stopped:{
        status_str = "stopped";
        break;
    }
    case MT::Status::Sleep:{
        status_str = "sleep";
        break;
    }
    }
    status_file << status_str << std::endl;
    status_file.close();
}

std::string MT::read_status_from_file()
{
    std::ifstream status(MT::Constants::status_file_name);
    if (!status.is_open()){
        std::string msg = "void MT::read_proces_id_from_file(): ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    std::string value;
    std::getline(status, value);
    return value;
}

void MT::display_brightness_signal_handler(int signal)
{
    if (signal == SIGQUIT){
        MT::Log::log().writeToLog("SIGQUIT received");
        working = false;
        std::unique_lock<std::mutex> lock(mtx);
        trigger_for_main_thread.notify_one();
    }
    else if (signal == SIGINT){
        MT::Log::log().writeToLog("SIGINT received");
        working = false;
        std::unique_lock<std::mutex> lock(mtx);
        trigger_for_main_thread.notify_one();
    }
    else if (signal == SIGTERM){
        MT::Log::log().writeToLog("SIGTERM received");
        working = false;
        std::unique_lock<std::mutex> lock(mtx);
        trigger_for_main_thread.notify_one();
    }
    else if (signal == SIGUSR1){
        MT::Log::log().writeToLog("SIGUSR1 received");
        read_new_brightness_values = true;
        std::unique_lock<std::mutex> lock(mtx);
        trigger_for_main_thread.notify_one();
    }
}

void MT::setup_brightness_values(std::array<short, 256> &values, int max_brightness)
{
    std::ifstream brightness_values("brightness_values");
    if (!brightness_values.is_open()){
        if (errno == ENOENT){
            MT::Log::log().writeToLog("File 'brightness_values' is missing, creating new one");
            std::ofstream file("brightness_values");
            if (!file.is_open()){
                std::string msg = "void MT::setup_brightness_values(std::array<short, 256> &values): ERROR on creating \'brightness_values\' file - ";
                msg += std::error_code(errno, std::generic_category()).message();
                throw std::fstream::failure(msg);
            }
            int brightness_step = max_brightness / 100 * 5;
            for(int i = 0, brightness_value = brightness_step * 2.5; i < 256; ++i){
                if (i > 12*19){
                    brightness_value = max_brightness;
                }
                else if (i % 12 == 0 && i > 23){
                    brightness_value += brightness_step;
                }
                values[i] = brightness_value;
                file << brightness_value << std::endl;
            }
            file.close();
            MT::Log::log().writeToLog("File 'brightness_values' with default values was succesfully created.");
        }
        else{
            std::string msg = "void MT::setup_brightness_values(std::array<short, 256> &values, int max_brightness): ERROR on opening \'brightness_values\' file for reading - ";
            msg += std::error_code(errno, std::generic_category()).message();
            throw std::fstream::failure(msg);
        }
    }
    else{
        MT::Log::log().writeToLog("Reading brightness values from file");
        for (int i = 0; i < 256; ++i){
            std::string value;
            std::getline(brightness_values, value);
            values[i] = std::stoi(value);
        }
        brightness_values.close();
        MT::Log::log().writeToLog("Brightness values was succesfully read");
    }
}
void MT::change_brightness_values(int percent, PlusOrMinus operand)
{
    std::ifstream brightness_values("brightness_values");
    if (!brightness_values.is_open()){
        std::string msg = "void MT::change_brightness_values(int percent, PlusOrMinus operand): ERROR on reading \'brightness values\' file - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    std::array<short, 256> values;
    MT::Log::log().writeToLog("Reading brightness values from file");
    for (int i = 0; i < 256; ++i){
        std::string value;
        std::getline(brightness_values, value);
        values[i] = std::stoi(value);
    }
    brightness_values.close();
    MT::Log::log().writeToLog("Brightness values was succesfully read");
    MT::Log::log().writeToLog("Calculating new values");
    for (short i : values){
        short change_value = i / 100 * percent;
        if (operand == PlusOrMinus::Increase){
            i += change_value;
        }
        else if (operand == PlusOrMinus::Decrease){
            i -= change_value;
        }
    }
    MT::Log::log().writeToLog("Writing new brightness values to file");
    std::ofstream new_brightness_values("brightness_values");
    if (!new_brightness_values.is_open()){
        std::string msg = "void MT::change_brightness_values(int percent, PlusOrMinus operand): ERROR on open \'brightness values\' file for writing - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    for (short i : values){
        new_brightness_values << i << std::endl;
    }
    new_brightness_values.close();
}
