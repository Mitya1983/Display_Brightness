#include "install_functions.h"
#include <fstream>
#include <filesystem>
#include <log.h>
#include "constant_names.h"
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

void MT::create_init_script()
{
    std::ofstream file("/etc/init.d/" + MT::Constants::executable_name);
    if (!file.is_open()){
        std::string msg = "bool MT::add_script_invoke_script_to_folder(const std::string &path): " +
                            std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    file << init_script();
    file.close();
}

std::string MT::init_script()
{
    std::string script = "#!/bin/sh\n\n"
                         "case $1 in\n"
                         "start)\n"
                         + MT::Constants::executable_name + " \"start\"\n"
                         ";;\n"
                         "stop)\n"
                         + MT::Constants::executable_name + " \"stop\"\n"
                         ";;\n"
                         "restart)\n"
                         + MT::Constants::executable_name + " \"stop\"\n"
                         "sleep 2\n"
                         + MT::Constants::executable_name + " \"start\"\n"
                         ";;\n"
                         "reload|force_reload)\n"
                         "#nothing to do\n"
                         ";;\n"
                         "status)\n"
                         + MT::Constants::executable_name + " \"status\"\n"
                         ";;\n"
                         "esac\n\n"
                         "exit 0";
    return script;

}

bool MT::folder_exists(const std::string &path)
{
    std::filesystem::directory_entry folder(path);

    return folder.exists();
}

void MT::clear_innstall(const Config &config)
{
    using file_or_directory = std::filesystem::directory_entry;
    file_or_directory install_dir(config.install_dir());
    if (install_dir.exists()){
        std::filesystem::remove_all(install_dir);
    }
    file_or_directory sleep_script("/lib/systemd/system-sleep/"+MT::Constants::executable_name);
    if (sleep_script.exists()){
        std::filesystem::remove(sleep_script);
    }
    file_or_directory init_script("/etc/init.d/"+MT::Constants::executable_name);
    if (init_script.exists()){
        std::filesystem::remove(init_script);
    }
    file_or_directory rc0_script("/etc/rc0.d/K01aa-"+MT::Constants::executable_name);
    if (rc0_script.exists()){
        std::filesystem::remove(rc0_script);
    }
    file_or_directory rc5_script("/etc/rc5.d/S99"+MT::Constants::executable_name);
    if (rc5_script.exists()){
        std::filesystem::remove(rc5_script);
    }
    file_or_directory rc6_script("/etc/rc6.d/K01aa-"+MT::Constants::executable_name);
    if (rc6_script.exists()){
        std::filesystem::remove(rc6_script);
    }
    file_or_directory bin_script("/usr/bin/"+MT::Constants::executable_name);
    if (bin_script.exists()){
        std::filesystem::remove(bin_script);
    }
}
