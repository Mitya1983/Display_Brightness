#ifndef CONFIG_H
#define CONFIG_H
#include <string>
namespace MT {

class Config
{
private:

    std::string _install_dir_name;
    std::string _display_state_file;
    std::string _display_brightness_file;
    std::string _actual_display_brightness_file;
    int _time_interval;
public:
    //CONSTRUCTORS
    Config() = default;
    Config(const Config&) = default;
    Config(Config&&) = default;
    //OPERATORS
    Config& operator=(const Config&) = default;
    Config& operator=(Config&&) = default;
    //SETTERS

    //GETTERS

    //FUNCTIONS
    void load_config();
    void write_config();
    //DESTRUCTOR
    ~Config() = default;
};

} // namespace MT
#endif // CONFIG_H
