#ifndef CONFIG_H
#define CONFIG_H
#include <string>
namespace MT {

class Config
{

private:

    std::string _install_dir_name;
    std::string _display_state_file;
    std::string _display_max_brightness_file;
    std::string _display_brightness_file;
    std::string _actual_display_brightness_file;
    int _time_interval;
    bool _ready;
    std::string _to_file(const std::string &code, const std::string &value);
public:
    //CONSTRUCTORS
    Config();
    Config(const Config&) = default;
    Config(Config&&) = default;
    //OPERATORS
    Config& operator=(const Config&) = default;
    Config& operator=(Config&&) = default;
    //SETTERS
    void set_install_dir_name(const std::string &install_dir);
    void set_display_state_file(const std::string &display_state_file);
    void set_display_max_brightness_file(const std::string &display_max_brightness_file);
    void set_display_brightness_file(const std::string &display_brightness_file);
    void set_actual_display_brightness_file(const std::string &actual_display_brightness_file);
    void set_time_interval(int time_interval);
    //GETTERS
    const std::string &install_dir() const;
    const std::string &display_state_file() const;
    const std::string &display_max_brightness_file() const;
    const std::string &display_brightness_file() const;
    const std::string &actual_display_brightness_file() const;
    int time_interval() const;
    [[nodiscard]] bool is_ready() const;
    //FUNCTIONS
    void load_config();
    void write_config();
    //DESTRUCTOR
    ~Config() = default;
};

} // namespace MT
#endif // CONFIG_H
