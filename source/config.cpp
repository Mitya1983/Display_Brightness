#include "config.h"
#include <fstream>

const std::string _install_dir_acronim = "idn";
const std::string _display_state_file_acronim = "dsf";
const std::string _display_max_brightness_file_acronim = "mbf";
const std::string _display_brightness_file_acronim = "dbf";
const std::string _actual_display_brightness_file_acronim = "abf";
const std::string _time_interval_acronim = "tin";

std::string MT::Config::_to_file(const std::string &code, const std::string &value)
{
    return code + '=' + value;
}

MT::Config::Config() :
    _install_dir_name("null"),
    _display_state_file("null"),
    _display_max_brightness_file("null"),
    _display_brightness_file("null"),
    _actual_display_brightness_file("null"),
    _time_interval(0),
    _ready(false)
{

}

void MT::Config::set_install_dir_name(const std::string &install_dir)
{
    _install_dir_name = install_dir;
}

void MT::Config::set_display_state_file(const std::string &display_state_file)
{
    _display_state_file = display_state_file;
}

void MT::Config::set_display_max_brightness_file(const std::string &display_max_brightness_file)
{
    _display_max_brightness_file = display_max_brightness_file;
}

void MT::Config::set_display_brightness_file(const std::string &display_brightness_file)
{
    _display_brightness_file = display_brightness_file;
}

void MT::Config::set_actual_display_brightness_file(const std::string &actual_display_brightness_file)
{
    _actual_display_brightness_file = actual_display_brightness_file;
}

void MT::Config::set_time_interval(int time_interval)
{
    _time_interval = time_interval;
}

const std::string &MT::Config::install_dir() const
{
    return _install_dir_name;
}

const std::string &MT::Config::display_state_file() const
{
    return _display_state_file;
}

const std::string &MT::Config::display_max_brightness_file() const
{
    return _display_max_brightness_file;
}

const std::string &MT::Config::display_brightness_file() const
{
    return _display_brightness_file;
}

const std::string &MT::Config::actual_display_brightness_file() const
{
    return _actual_display_brightness_file;
}

int MT::Config::time_interval() const
{
    return _time_interval;
}

bool MT::Config::is_ready() const
{
    return _ready;
}

void MT::Config::load_config()
{
    _ready = true;
    std::ifstream config("config");
    if (!config.is_open()){
        std::string msg = "void MT::Config::load_config(): ERROR on opening \'config\' file for reading - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    std::string config_line;
    while (std::getline(config, config_line)){
        if (config_line.find(_install_dir_acronim, 0) != std::string::npos){
            _install_dir_name = config_line.substr(4);
        }
        else if (config_line.find(_display_state_file_acronim, 0) != std::string::npos){
            _display_state_file = config_line.substr(4);
        }
        else if (config_line.find(_display_max_brightness_file_acronim, 0) != std::string::npos){
            _display_max_brightness_file = config_line.substr(4);
        }
        else if (config_line.find(_display_brightness_file_acronim, 0) != std::string::npos){
            _display_brightness_file = config_line.substr(4);
        }
        else if (config_line.find(_actual_display_brightness_file_acronim, 0) != std::string::npos){
            _actual_display_brightness_file = config_line.substr(4);
        }
        else if (config_line.find(_time_interval_acronim, 0) != std::string::npos){
            _time_interval = std::stoi(config_line.substr(4));
        }
    }
    if (_install_dir_name == "null" || _display_state_file == "null" || _display_max_brightness_file == "null"
        || _display_brightness_file == "null" || _actual_display_brightness_file == "null" || _time_interval == 0){
        _ready = false;
    }
    config.close();
}

void MT::Config::write_config()
{
    std::ofstream config("config");
    if (!config.is_open()){
        std::string msg = "void MT::Config::load_config(): ERROR on opening \'config\' file for writing - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
        config << _to_file(_install_dir_acronim, _install_dir_name) << std::endl;
        config << _to_file(_display_state_file_acronim, _display_state_file) << std::endl;
        config << _to_file(_display_max_brightness_file_acronim, _display_max_brightness_file) << std::endl;
        config << _to_file(_display_brightness_file_acronim, _display_brightness_file) << std::endl;
        config << _to_file(_actual_display_brightness_file_acronim, _actual_display_brightness_file) << std::endl;
        config << _to_file(_time_interval_acronim, std::to_string(_time_interval)) << std::endl;
    config.close();
}
