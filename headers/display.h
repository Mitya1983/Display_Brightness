#ifndef DISPLAY_H
#define DISPLAY_H
#include <string>
namespace MT{

class Display
{
private:
    std::string _max_brightness_file;
    std::string _cur_brightness_file;
    bool _is_on;

    int _min_brightness;
    int _max_brightness;
    int _cur_brightness;
    int _cur_brightness_proc;


    int _read_max_brightness();
    int _read_cur_brightness();
    int _cur_brightness_to_proc();
    bool _read_display_state();
public:
    //CONSTRUCTORS
    Display();
    Display(const Display&) = delete;
    Display(Display&&) = delete;
    //OPERATORS
    Display& operator=(const Display&) = delete;
    Display& operator=(Display&&) = delete;
    //SETTERS
    void set_max_brightness_file(const std::string &file);
    void set_cur_brightness_file(const std::string &file);
    [[nodiscard]] bool set_cur_brightness(int brightness);
    void set_cur_brightness_from_file();
    void set_is_on(bool value);
    //GETTERS
    [[nodiscard]] int max_brightness() const noexcept;
    [[nodiscard]] int cur_brightness() const noexcept;
    [[nodiscard]] int cur_brightness_proc() const noexcept;
    [[nodiscard]] bool is_on() const noexcept;
    //FUNCTIONS

    //DESTRUCTOR
    ~Display() = default;
};

}//namespace MT
#endif // DISPLAY_H
