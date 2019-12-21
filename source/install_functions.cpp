#include "install_functions.h"
#include <fstream>
#include <filesystem>
#include "log.h"
bool MT::write_brightness_file_path()
{
    bool found = false;
    std::filesystem::path path("/sys/class/backlight");
    std::filesystem::directory_entry backlight_folder(path);
    if (backlight_folder.exists() && backlight_folder.is_directory()){
        MT::Log::log().writeToLog("Searching for subfolders in '/sys/class/backlight' folder");
        for (auto &i : std::filesystem::directory_iterator(backlight_folder)){
            if (i.is_directory()){
                MT::Log::log().writeToLog("\'" + i.path().filename().string() + "\' folder was found");
                MT::Log::log().writeToLog("Searching for 'brightness' file");
                for (auto &j : std::filesystem::directory_iterator(i)){
                    if (j.is_regular_file()){
                        if (j.path().filename().string() == "brightness"){
                            found = true;
                            MT::Log::log().writeToLog("\'brightness\' file was found");
                            std::ofstream brightness_path(std::filesystem::current_path().string() + "/brightness_path", std::ios::trunc);
                            if (!brightness_path.is_open()){
                                std::string msg = "bool MT::write_brightness_file_path(): ERROR on create \'brightness_path\' file - ";
                                msg += std::error_code(errno, std::generic_category()).message();
                                throw std::fstream::failure(msg);
                            }
                            brightness_path << j.path() << std::endl;
                            brightness_path.close();
                            MT::Log::log().writeToLog("\'brightness_path\' file was succesfully created");
                            MT::Log::log().writeToLog("Changing permissions of \'brightness\' file");
                            break;
                        }
                    }
                }
            }
            if (found){
                break;
            }
        }
    }
    if (!found){
        MT::Log::log().writeToLog("\'brightness\' file wasn't found");
    }
    return found;
}

bool MT::write_actual_brightness_file_path()
{
    bool found = false;
    std::filesystem::path path("/sys/class/backlight");
    std::filesystem::directory_entry backlight_folder(path);
    if (backlight_folder.exists() && backlight_folder.is_directory()){
        MT::Log::log().writeToLog("Searching for subfolders in '/sys/class/backlight' folder");
        for (auto &i : std::filesystem::directory_iterator(backlight_folder)){
            if (i.is_directory()){
                MT::Log::log().writeToLog("\'" + i.path().filename().string() + "\' folder was found");
                MT::Log::log().writeToLog("Searching for 'actual_brightness' file");
                for (auto &j : std::filesystem::directory_iterator(i)){
                    if (j.is_regular_file()){
                        if (j.path().filename().string() == "actual_brightness"){
                            found = true;
                            MT::Log::log().writeToLog("\'actual_brightness\' file was found");
                            std::ofstream actual_brightness_path(std::filesystem::current_path().string() + "/actual_brightness_path", std::ios::trunc);
                            if (!actual_brightness_path.is_open()){
                                std::string msg = "bool MT::write_brightness_file_path(): ERROR on create \'actual_brightness_path\' file - ";
                                msg += std::error_code(errno, std::generic_category()).message();
                                throw std::fstream::failure(msg);
                            }
                            actual_brightness_path << j.path() << std::endl;
                            actual_brightness_path.close();
                            MT::Log::log().writeToLog("\'actual_brightness_path\' file was succesfully created");
                            break;
                        }
                    }
                }
            }
            if (found){
                break;
            }
        }
    }
    if (!found){
        MT::Log::log().writeToLog("\'actual_brightness\' file wasn't found");
    }
    return found;
}

bool MT::write_max_brightness_file_path()
{
    bool found = false;
    std::filesystem::path path("/sys/class/backlight");
    std::filesystem::directory_entry backlight_folder(path);
    if (backlight_folder.exists() && backlight_folder.is_directory()){
        MT::Log::log().writeToLog("Searching for subfolders in '/sys/class/backlight' folder");
        for (auto &i : std::filesystem::directory_iterator(backlight_folder)){
            if (i.is_directory()){
                MT::Log::log().writeToLog("\'" + i.path().filename().string() + "\' folder was found");
                MT::Log::log().writeToLog("Searching for 'max_brightness' file");
                for (auto &j : std::filesystem::directory_iterator(i)){
                    if (j.is_regular_file()){
                        if (j.path().filename().string() == "max_brightness"){
                            found = true;
                            MT::Log::log().writeToLog("\'max_brightness\' file was found");
                            std::ofstream max_brightness_path(std::filesystem::current_path().string() + "/max_brightness_path", std::ios::trunc);
                            if (!max_brightness_path.is_open()){
                                std::string msg = "bool MT::write_max_brightness_file_path(): ERROR on create \'max_brightness_path\' file - ";
                                msg += std::error_code(errno, std::generic_category()).message();
                                throw std::fstream::failure(msg);
                            }
                            max_brightness_path << j.path() << std::endl;
                            max_brightness_path.close();
                            MT::Log::log().writeToLog("\'max_brightness_path\' file was succesfully created");
                            break;
                        }
                    }
                }
            }
            if (found){
                break;
            }
        }
    }
    if (!found){
        MT::Log::log().writeToLog("\'actual_brightness\' file wasn't found");
    }
    return found;
}

bool MT::write_display_state_file_path()
{
    bool succeded = false;
    std::filesystem::directory_entry dpms("/sys/class/drm/card0/card0-eDP-1/dpms");
    if (dpms.exists()){
        std::
    }
    return succeded;
}

void MT::create_invoke_script(const std::string &path)
{
    std::ofstream file(path);
    if (!file.is_open()){
        std::string msg = "bool MT::add_script_invoke_script_to_folder(const std::string &path): " +
                            std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    file << invoke_script();
    file.close();
}

std::string MT::invoke_script()
{
    std::string script = "#!/bin/sh\n\n"
                         "case $1 in\n"
                         "\"pre\")\n"
                         "/home/mitya/Projects/Display_Brightness/Build/Display_Brightness \"stop\"\n"
                         ";;\n"
                         "\"post\")\n"
                         "/home/mitya/Projects/Display_Brightness/Build/Display_Brightness \"run\"\n"
                         ";;\n"
                         "esac";

    return script;

}

bool MT::folder_exists(const std::string &path)
{
    std::filesystem::directory_entry folder(path);

    return folder.exists();
}

void MT::clear_install()
{

}
