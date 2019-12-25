#include "install_functions.h"
#include <fstream>
#include <filesystem>
#include <log.h>
void MT::write_brightness_file_path(Config &config)
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
                            config.set_display_brightness_file(j.path().string());
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
        config.set_display_brightness_file("null");
    }
}

void MT::write_actual_brightness_file_path(Config &config)
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
                            config.set_actual_display_brightness_file(j.path().string());
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
        config.set_actual_display_brightness_file("null");
    }
}

void MT::write_max_brightness_file_path(Config &config)
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
                            config.set_display_max_brightness_file(j.path().string());
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
        MT::Log::log().writeToLog("\'max_brightness\' file wasn't found");
        config.set_display_max_brightness_file("null");
    }
}

void MT::write_display_state_file_path(Config &config)
{
    if (MT::folder_exists("/sys/class/drm/card0/card0-eDP-1/dpms")){
        config.set_display_state_file("/sys/class/drm/card0/card0-eDP-1/dpms");
    }
    else{
        std::filesystem::directory_entry card0("/sys/class/drm/card0");
        bool found = false;
        if (card0.exists() && card0.is_directory()){
            for (auto &i : std::filesystem::directory_iterator(card0)){
                if (i.is_directory() && i.path().string().find("card0-eDP", 0) != std::string::npos){
                    for (auto &j : std::filesystem::directory_iterator(i)){
                        if (j.is_regular_file() && j.path().filename() == "dpms"){
                            found = true;
                            MT::Log::log().writeToLog("\'dpms\' file was found");
                            config.set_display_state_file(j.path().string());
                            break;
                        }
                    }
                }
                if (found){
                    break;
                }
            }
        }
        if (!found){
            MT::Log::log().writeToLog("\'dpms\' file wasn't found");
            config.set_display_state_file("null");
        }
    }
}

void MT::create_invoke_script(const std::string &path)
{
    std::string file_path = path;
    if (file_path[file_path.length()] == '/'){
        file_path += "dispbr";
    }
    else{
        file_path += "/dispbr";
    }
    std::ofstream file(file_path);
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
