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

//Global variable for signal handling
bool working;
std::condition_variable trigger_for_main_thread;
std::mutex mtx;
void MT::brightness_adjust()
{
    //Setting up the system signal handling
    struct sigaction signal_action;
    signal_action.sa_handler = &display_brightness_signal_handler;
    if (sigaction(SIGQUIT, &signal_action, nullptr) == -1){
        std::string msg = "Error on catching SIGQUIT: ";
        msg += std::error_code(errno, std::generic_category()).message();
        Log::log().writeToLog(msg);
        Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGINT, &signal_action, nullptr) == -1){
        std::string msg = "Error on catching SIGINT: ";
        msg += std::error_code(errno, std::generic_category()).message();
        Log::log().writeToLog(msg);
        Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }
    if (sigaction(SIGTERM, &signal_action, nullptr) == -1){
        std::string msg = "Error on catching SIGTERM: ";
        msg += std::error_code(errno, std::generic_category()).message();
        Log::log().writeToLog(msg);
        Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }
    /******************************************************************************************************************/
    //VARIABLES

    //Variable to handle display brightness
    std::unique_ptr<Display> display = std::make_unique<Display>();

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
    unsigned int time_interval;
    try {
        time_interval = get_time_interval_from_file();
    } catch (std::fstream::failure &e) {
        Log::log().writeToLog(e.what());
        Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }

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

    //Writing process ID to file
    try {
        write_process_id_to_file(static_cast<int>(getpid()));
    } catch (std::fstream::failure &e) {
        Log::log().writeToLog(e.what());
        Log::log().writeToLog("Exiting with failure");
        exit(EXIT_FAILURE);
    }
    working = true;
    //Lunching notifier thread
    std::thread notifier(brightness_change_notifier, std::ref(trigger_for_main_thread),
                         std::ref(brightness_was_changed_by_user), std::ref(brightness_was_changed_by_app), std::move(signal_send));

    //Main loop
    Log::log().writeToLog("Starting luminocity check");
    while (true){
        if (!brightness_was_changed_by_user){
            try {
                image->capture_frame();
                if (first_image){
                    image->capture_frame();
                    first_image = false;
                }
            } catch (std::runtime_error &e) {
                Log::log().writeToLog(e.what());
                write_process_id_to_file(0);
                signal_to_stop.set_value();
                notifier.join();
                Log::log().writeToLog("Exiting with failure");
                exit(EXIT_FAILURE);
            }
            try {
                bool brightness_was_changed = display->set_cur_brightness(brightness_values[image->average_grayscale_value()]);
                if (brightness_was_changed){
                    times_brightness_wasnt_changed = 0;
                    time_interval = get_time_interval_from_file();
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
                Log::log().writeToLog(e.what());
                write_process_id_to_file(0);
                signal_to_stop.set_value();
                notifier.join();
                Log::log().writeToLog("Exiting with failure");
                exit(EXIT_FAILURE);
            }
        }
        else{
            mtx.lock();
            brightness_was_changed_by_user = false;
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
        if (brightness_was_changed_by_user){
            display->set_cur_brightness_from_file();

        }
    }
    write_process_id_to_file(0);
    signal_to_stop.set_value();
    notifier.join();
    Log::log().writeToLog("Exiting luminocity check");
}

void MT::brightness_change_notifier(std::condition_variable &cv, bool &signal_to_read_brightness,
                                    bool &brightness_was_set_by_app, std::future<void> signal_to_stop)
{
    Log::log().writeToLog("Starting notify thread");
    int notifier = inotify_init1(IN_NONBLOCK);
    int file_to_watch = inotify_add_watch(notifier, "/sys/class/backlight/intel_backlight/actual_brightness", IN_MODIFY);
    if (file_to_watch < 0){
        std::string msg = "void MT::brightness_change_notifier(std::condition_variable &cv, bool &signal_to_read_brightness, std::future<void> signal_to_stop): ";
        msg += std::error_code(errno, std::generic_category()).message();
        Log::log().writeToLog(msg);
        Log::log().writeToLog("Exiting notify thread with failure");
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
    Log::log().writeToLog("Exiting notify thread");
}

void MT::write_process_id_to_file(int process_id)
{
    std::ofstream status_file("status", std::ios_base::trunc);
    if (!status_file.is_open()){
        std::string msg = "void write_process_id_to_file(int process_id): ERROR on opening \'status\' file for writing - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    status_file << process_id << std::endl;
    status_file.close();
}

void MT::display_brightness_signal_handler(int signal)
{
    std::string msg;
    if (signal == SIGQUIT){
        msg = "SIGQUIT received";
    }
    else if (signal == SIGINT){
        msg = "SIGINT received";
    }
    else{
        msg = "SIGTERM received";
    }
    Log::log().writeToLog(msg);
    working = false;
    std::unique_lock<std::mutex> lock(mtx);
    trigger_for_main_thread.notify_one();
}

int MT::get_time_interval_from_file()
{
    int value_to_return = 30;
    std::string file_name = "time_interval";
    std::ifstream time_interval(file_name);
    if (!time_interval.is_open()){
        if (errno == ENOENT){
            Log::log().writeToLog("File 'time_interval' is missing, creating new one");
            std::ofstream file(file_name);
            if (!file.is_open()){
                std::string msg = "int MT::get_time_interval_from_file(): ERROR on creating \'time_interval\' file for - ";
                msg += std::error_code(errno, std::generic_category()).message();
                throw std::fstream::failure(msg);
            }
            file << value_to_return << std::endl;
            file.close();
            Log::log().writeToLog("File 'time_interwal' with default starting value (30 sec) was succesfully created.");
        }
        else{
            std::string msg = "int MT::get_time_interval_from_file(): ERROR on opening \'time_interval\' file for reading - ";
            msg += std::error_code(errno, std::generic_category()).message();
            throw std::fstream::failure(msg);
        }
    }
    else{
        Log::log().writeToLog("Reading time interval from file");
        std::string value;
        std::getline(time_interval, value);
        time_interval.close();
        value_to_return = std::stoi(value);
        Log::log().writeToLog("Time interval was read succesfully");
    }
    return value_to_return;
}

void MT::setup_brightness_values(std::array<short, 256> &values, int max_brightness)
{
    std::string file_name = "brightness_values";
    std::ifstream brightness_values(file_name);
    if (!brightness_values.is_open()){
        if (errno == ENOENT){
            Log::log().writeToLog("File 'brightness_values' is missing, creating new one");
            std::ofstream file(file_name);
            if (!file.is_open()){
                std::string msg = "void MT::setup_brightness_values(std::array<short, 256> &values): ERROR on creating \'time_interval\' file for - ";
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
            Log::log().writeToLog("File 'brightness_values' with default values was succesfully created.");
        }
        else{
            std::string msg = "void MT::setup_brightness_values(std::array<short, 256> &values, int max_brightness): ERROR on opening \'brightness_values\' file for reading - ";
            msg += std::error_code(errno, std::generic_category()).message();
            throw std::fstream::failure(msg);
        }
    }
    else{
        Log::log().writeToLog("Reading brightness values from file");
        for (int i = 0; i < 256; ++i){
            std::string value;
            std::getline(brightness_values, value);
            values[i] = std::stoi(value);
        }
        brightness_values.close();
        Log::log().writeToLog("Brightness values was read succesfully");
    }
}
