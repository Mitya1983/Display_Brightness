#include "display.h"
#include <fstream>
#include <system_error>


int MT::Display::_read_max_brightness()
{
    std::ifstream in(_max_brightness_file);
    if (!in.is_open()){
        std::string msg = "int Display::_read_max_brightness(): ERROR opening file ";
        msg += _max_brightness_file;
        msg += " for reading - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    std::string value;
    std::getline(in, value);
    in.close();
    int brightness = std::stoi(value);
    return brightness;
}

int MT::Display::_read_cur_brightness()
{
    std::ifstream in(_cur_brightness_file);
    if (!in.is_open()){
        std::string msg = "int Display::_read_max_brightness(): ERROR on opening file ";
        msg += _cur_brightness_file;
        msg += " for reading - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    std::string value;
    std::getline(in, value);
    in.close();
    int brightness = std::stoi(value);
    return brightness;
}

int MT::Display::_cur_brightness_to_proc()
{
    float percent = static_cast<float>(_cur_brightness) / static_cast<float>(_max_brightness) * 100.00F;
    int int_percent = static_cast<int>(percent);

    return int_percent;
}

bool MT::Display::_read_display_state()
{
    std::ifstream dpms(_state_file);
    if (!dpms.is_open()){
        std::string msg = "bool Display::_read_display_state(): ERROR on opening \'dmps\' file - ";
        msg += std::error_code(errno, std::generic_category()).message();
        throw std::fstream::failure(msg);
    }
    std::string line;
    std::getline(dpms, line);
    dpms.close();
    return line == "On" ? true : false;
}

MT::Display::Display() :
    _max_brightness_file("/sys/class/backlight/intel_backlight/max_brightness"),
    _cur_brightness_file("/sys/class/backlight/intel_backlight/brightness"),
    _min_brightness(0)
{
    _max_brightness = _read_max_brightness();
    _cur_brightness = _read_cur_brightness();
    _cur_brightness_proc = _cur_brightness_to_proc();
    _is_on = _read_display_state();
}

void MT::Display::set_max_brightness_file(const std::string &file)
{
    if (file == ""){
        throw std::invalid_argument("void Display::set_max_brightness_file(const std::string &file): Empty string was provided");
    }
    _max_brightness_file = file;
}

void MT::Display::set_cur_brightness_file(const std::string &file)
{
    if (file == ""){
        throw std::invalid_argument("void Display::set_cur_brightness_file(const std::string &file): Empty string was provided");
    }
    _cur_brightness_file = file;
}

void MT::Display::set_state_file(const std::string &file)
{
    if (file == ""){
        throw std::invalid_argument("void MT::Display::set_state_file(const std::string &file): Empty string was provided");
    }
    _state_file = file;
}

bool MT::Display::set_cur_brightness(int brightness)
{
    _cur_brightness = _read_cur_brightness();
    if (_cur_brightness != brightness){
        std::ofstream out(_cur_brightness_file, std::ios_base::binary | std::ios_base::trunc);
        if (!out.is_open()){
            std::string msg = "void Display::set_cur_brightness(int brightness): ERROR on opening file ";
            msg += _cur_brightness_file;
            msg += " for writing - ";
            msg += std::error_code(errno, std::generic_category()).message();
            throw std::fstream::failure(msg);
        }
        brightness > _max_brightness ? _cur_brightness = _max_brightness : (brightness < 0 ? _cur_brightness = 0 : _cur_brightness = brightness);

        out << _cur_brightness;
        out.close();
        _cur_brightness_proc = _cur_brightness_to_proc();
        return true;
    }

    return false;
}

void MT::Display::set_cur_brightness_from_file()
{
    _cur_brightness = _read_cur_brightness();
    _cur_brightness_proc = _cur_brightness_to_proc();
}

void MT::Display::set_is_on(bool value)
{
    _is_on = value;
}

int MT::Display::max_brightness() const noexcept
{
    return _max_brightness;
}

int MT::Display::cur_brightness() const noexcept
{
    return _cur_brightness;
}

int MT::Display::cur_brightness_proc() const noexcept
{
    return _cur_brightness_proc;
}

bool MT::Display::is_on() const noexcept
{
    return _is_on;
}
