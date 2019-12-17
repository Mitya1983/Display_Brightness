#ifndef FUNCTION_TO_FORK_H
#define FUNCTION_TO_FORK_H
#include <condition_variable>
#include <array>
#include <future>
namespace MT{

void brightness_adjust();
void brightness_change_notifier(std::condition_variable &cv, bool &signal_to_read_brightness, bool &brightness_was_set_by_app,
                                std::future<void> signal_to_stop);

//Helpers
void write_process_id_to_file(int process_id);
void display_brightness_signal_handler(int signal);
int get_time_interval_from_file();
void setup_brightness_values(std::array<short, 256> &values, int max_brightness);

}//namespace MT
#endif // FUNCTION_TO_FORK_H
