#include <iostream>
#include <config.h>
#include <install_functions.h>
#include <fstream>
#include <log.h>
#include <constant_names.h>
int main(int argc, char **argv)
{
    MT::Config config;
    std::string log_file = std::filesystem::current_path().string() + "/install_log.txt";
    MT::Log::log().set_file_name(log_file);
    //Create install directory
    switch (argc){
    case 1:{
        config.set_install_dir_name("/opt/dispbr");
        break;
    }
    case 2:{
        std::string install_dir = argv[1];
        if (install_dir[install_dir.length()] == '/'){
            install_dir += MT::Constants::executable_name;
        }
        else{
            install_dir += "/" + MT::Constants::executable_name;
        }
        config.set_install_dir_name(install_dir);
        break;
    }
    default:{
        std::cout << "To many arguments were provided. Expected 1 but " << argc - 1 << " were provided.";
        return 0;
    }
    }
    MT::Log::log().writeToLog("Install directory is set to" + config.install_dir());
    std::cout << "Creating install directory: ";
    std::filesystem::path install_dir(config.install_dir());
    try {
        std::filesystem::create_directory(install_dir);
        std::cout << "Done" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;

        return 0;
    }
    std::filesystem::path source_path(std::filesystem::current_path());
    std::filesystem::current_path(install_dir);
    std::cout << "Searching for \'brightness\' file: ";
    //Serching for brightness file and writing path to config
    MT::write_brightness_file_path(config);
    if (config.display_brightness_file() != "null"){
        std::cout << "Found" << std::endl;
        MT::Log::log().writeToLog("\'brightness\' file was found: " + config.display_brightness_file());
    }
    else{
        std::cout << " NOT found. Must to be set by user!" << std::endl;
        MT::Log::log().writeToLog("\'brightness\' file wasn't found.");
    }
    //Serching for max_brightness file and writing path to config
    std::cout << "Searching for \'max_brightness\' file: ";
    MT::write_max_brightness_file_path(config);
    if (config.display_max_brightness_file() != "null"){
        std::cout << "Found" << std::endl;
        MT::Log::log().writeToLog("\'max_brightness\' file was found: " + config.display_max_brightness_file());
    }
    else{
        std::cout << " NOT found. Must to be set by user!" << std::endl;
        MT::Log::log().writeToLog("\'max_brightness\' file wasn't found.");
    }
    //Serching for actual_brightness file and writing path to config
    std::cout << "Searching for \'actual_brightness\' file: ";
    MT::write_actual_brightness_file_path(config);
    if (config.actual_display_brightness_file() != "null"){
        std::cout << "Found" << std::endl;
        MT::Log::log().writeToLog("\'actual_brightness\' file was found: " + config.actual_display_brightness_file());
    }
    else{
        std::cout << " NOT found. Must to be set by user!" << std::endl;
        MT::Log::log().writeToLog("\'actual_brightness\' file wasn't found.");
    }
    //Serching for dpms file and writing path to config
    std::cout << "Searching for \'dpms\' file: ";
    MT::write_display_state_file_path(config);
    if (config.display_state_file() != "null"){
        std::cout << "Found" << std::endl;
        MT::Log::log().writeToLog("\'dpms\' file was found: " + config.display_state_file());
    }
    else{
        std::cout << " NOT found. Must to be set by user!" << std::endl;
        MT::Log::log().writeToLog("\'dpms\' file wasn't found.");
    }
    //Writing default time interval to config
    config.set_time_interval(60);
    //Creating config file
    std::cout << "Creating \'config\' file: ";
    try {
        config.write_config();
        std::cout << "Done" << std::endl;
        MT::Log::log().writeToLog("Config file created");
    } catch (std::fstream::failure &e) {
        std::cout << e.what() << std::endl;
        MT::Log::log().writeToLog(e.what());
        MT::Log::log().writeToLog("Deleting install directory");
        std::cout << "Clearing install: ";
        std::filesystem::remove(install_dir);
        MT::Log::log().writeToLog("Install direcory was deleted.");
        std::cout << "Done" << std::endl;
        return 0;
    }
    //Copying executable to install dir
    std::cout << "Copying executable to install directory: ";
    std::string executable = source_path / MT::Constants::executable_name;
    try {
        std::filesystem::copy(executable, std::filesystem::current_path());
        std::cout << "Done" << std::endl;
        MT::Log::log().writeToLog("Executable copied");
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
        MT::Log::log().writeToLog(e.what());
        MT::Log::log().writeToLog("Deleting install directory");
        std::cout << "Clearing install: ";
        auto deleted_files = std::filesystem::remove_all(install_dir);
        MT::Log::log().writeToLog(std::to_string(deleted_files) + " was deleted.");
        std::cout << "Done" << std::endl;
        return 0;
    }
    //Creating script in system-sleep directory
    std::cout << "Creating invoke script in \'system-sleep\' folder: ";
    if (MT::folder_exists("/lib/systemd/system-sleep")){
        try {
            MT::create_invoke_script("/lib/systemd/system-sleep");
            std::filesystem::permissions("/lib/systemd/system-sleep/" + MT::Constants::executable_name,
                                         std::filesystem::perms::owner_exec,
                                         std::filesystem::perm_options::add);
            std::cout << "Done" << std::endl;
        } catch (std::fstream::failure &e) {
            std::cout << e.what() << std::endl;
        }
    }
    else{
        std::cout << "/lib/systemd/system-sleep folder not found. Service start-stop logic will not work" << std::endl;
    }
    //Creating script in system-shutdown directory
    std::cout << "Creating invoke script in \'system-shutdown\' folder: ";
    if (MT::folder_exists("/lib/systemd/system-shutdown")){
        try {
            MT::create_invoke_script("/lib/systemd/system-shutdown");
            std::filesystem::permissions("/lib/systemd/system-shutdown/" + MT::Constants::executable_name,
                                         std::filesystem::perms::owner_exec,
                                         std::filesystem::perm_options::add);
            std::cout << "Done" << std::endl;
        } catch (std::fstream::failure &e) {
            std::cout << e.what() << std::endl;
            MT::Log::log().writeToLog(e.what());
        }
    }
    else{
        std::cout << "/lib/systemd/system-shutdown folder not found. Service start-stop logic will not work" << std::endl;
    }
    //Creating script in /usr/bin
    std::cout << "Creating script in /usr/bin: ";
    std::ofstream script("/usr/bin/" + MT::Constants::executable_name);
    if (!script.is_open()){
        std::cout << std::error_code(errno, std::generic_category()).message() << std::endl;
        MT::Log::log().writeToLog(std::error_code(errno, std::generic_category()).message());
        std::cout << "Unable to create invoke script in /usr/bin folder." << std::endl;
    }
    std::string script_code = "#!/bin/sh\n\ncd " + config.install_dir() + "\n./" + MT::Constants::executable_name + " $@";
    script << script_code;
    script.close();
    try {
        std::filesystem::permissions("/usr/bin/" + MT::Constants::executable_name,
                                     std::filesystem::perms::others_exec,
                                     std::filesystem::perm_options::add);
        std::cout << "Done" << std::endl;
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
        MT::Log::log().writeToLog(e.what());
    }
    //Creating status file in install directory
    std::cout << "Creating \'status\' file: ";
    std::ofstream file(MT::Constants::status_file_name, std::ios_base::trunc);
    if (!file.is_open()){
        std::cout << "Error on open \'status\' file for writing: " << std::error_code(errno, std::generic_category()).message() << std::endl;
        MT::Log::log().writeToLog("Error on open \'status\' file for writing: " + std::error_code(errno, std::generic_category()).message());
    }
    else{
        file << "stopped" << std::endl;
        file.close();
        std::cout << "Done" << std::endl;
    }
    //Creating log file in install directory
    std::cout << "Creating \'log.txt\' file: ";
    file.open(MT::Constants::log_file_name, std::ios_base::trunc);
    if (!file.is_open()){
        std::cout << "Error on open \'status\' file for writing: " << std::error_code(errno, std::generic_category()).message() << std::endl;
        MT::Log::log().writeToLog("Error on open \'status\' file for writing: " + std::error_code(errno, std::generic_category()).message());
    }
    else{
        file.close();
        try {
            std::filesystem::permissions(config.install_dir() + '/' + MT::Constants::log_file_name,
                                         std::filesystem::perms::others_write,
                                         std::filesystem::perm_options::add);
            std::cout << "Done" << std::endl;
        } catch (std::filesystem::filesystem_error &e) {
            std::cout << e.what() << std::endl;
            MT::Log::log().writeToLog(e.what());
        }
    }
    //Copying uninstall executable to install directory
    std::cout << "Copying uninstall_executable to install directory: ";
    std::string uninstall_executable = source_path / "Uninstall";
    try {
        std::filesystem::copy(uninstall_executable, std::filesystem::current_path());
        std::cout << "Done" << std::endl;
        MT::Log::log().writeToLog("Uninstall_executable copied");
    } catch (std::filesystem::filesystem_error &e) {
        std::cout << e.what() << std::endl;
        MT::Log::log().writeToLog(e.what());
        std::cout << "Uninstall_executable wasn't copied. You can copy it from source." << std::endl;
        return 0;
    }
    return 0;
}
