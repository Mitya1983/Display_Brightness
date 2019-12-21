#ifndef FUNCTION_TO_FORK_H
#define FUNCTION_TO_FORK_H
#include <condition_variable>
#include <array>
#include <future>
namespace MT{

enum class Status{
    Stopped,
    Running,
    Sleep
};

enum class PlusOrMinus{
    Increase,
    Decrease
};

void brightness_adjust();
void brightness_change_notifier(std::condition_variable &cv, bool &signal_to_read_brightness, bool &brightness_was_set_by_app,
                                std::future<void> signal_to_stop);

//HELPERS
void write_pid_to_file();
pid_t read_pid_from_file();
void write_status_to_file(Status status);
std::string read_status_from_file();
void display_brightness_signal_handler(int signal);
int get_time_interval_from_file();
void setup_brightness_values(std::array<short, 256> &values, int max_brightness);
void change_brightness_values(int percent, PlusOrMinus operand);
std::string read_brightness_file_path();
std::string read_actual_brightness_file_path();
std::string read_max_brightness_file_path();

}//namespace MT
#endif // FUNCTION_TO_FORK_H
