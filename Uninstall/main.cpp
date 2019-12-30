#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/types.h>
#include <signal.h>
#include <constant_names.h>
int main()
{
    /*
     * 1. Stop service.
     * 2. Delete simlink.
     * 3. Delete script in system-shutdown folder.
     * 4. Delete script in system-sleep folder.
     * 5. Delete install directory folder.
    */
    std::cout << "Reading service status: ";
    std::ifstream file;
    file.open(MT::Constants::status_file_name);
    if (!file.is_open()){
        std::cout << "Error: " << std::error_code(errno, std::generic_category()).message() << std::endl;
        return 1;
    }
    std::string status;
    std::getline(file, status);
    file.close();
    if (status == "running" || status == "sleep"){
        std::cout << "Stopping service: ";
        file.open(MT::Constants::pid_file_name);
        if (!file.is_open()){
            std::cout << "Error: " << std::error_code(errno, std::generic_category()).message() << std::endl;
            return 1;
        }
        std::string pid;
        std::getline(file, pid);
        file.close();
        pid_t service_id = std::stoi(pid);
        int kill_status = kill(service_id, SIGQUIT);
        if (kill_status < 0){
            std::string msg = "kill() error: ";
            msg += std::error_code(errno, std::generic_category()).message();
            std::cout << msg << std::endl;
            return 1;
        }
        std::cout << "Done" << std::endl;
    }
    std::cout << "Done" << std::endl;
    std::cout << "Deleting script in /usr/bin: ";
    try {
        std::filesystem::remove("/usr/bin/" + MT::Constants::executable_name);
        std::cout << "Done" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    if (std::filesystem::directory_entry("/lib/systemd/system-sleep/" + MT::Constants::executable_name).exists()){
        std::cout << "Deleting invoke scirpt in /lib/systemd/system-sleep: ";
        try {
            std::filesystem::remove("/lib/systemd/system-sleep/" + MT::Constants::executable_name);
            std::cout << "Done" << std::endl;
        } catch (std::filesystem::filesystem_error &e) {
            std::cout << e.what() << std::endl;
            return 1;
        }
    }
    std::cout << "Deleting symlink in /etc/rc0.d/: ";
    try {
        std::filesystem::remove("/etc/rc0.d/K01aa-" + MT::Constants::executable_name);
        std::cout << "Done" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::cout << "Deleting symlink in /etc/rc5.d/: ";
    try {
        std::filesystem::remove("/etc/rc5.d/S99" + MT::Constants::executable_name);
        std::cout << "Done" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::cout << "Deleting symlink in /etc/rc6.d/: ";
    try {
        std::filesystem::remove("/etc/rc6.d/K01aa-" + MT::Constants::executable_name);
        std::cout << "Done" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::cout << "Deleting init script in /etc/init.d/: ";
    try {
        std::filesystem::remove("/etc/init.d/" + MT::Constants::executable_name);
        std::cout << "Done" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    std::filesystem::path working_path = std::filesystem::current_path();
    std::cout << "Deleting " << working_path << ": ";
    try {
        std::filesystem::remove_all(working_path);
        std::cout << "Done" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
