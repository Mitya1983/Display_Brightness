#ifndef INSTALL_FUNCTIONS_H
#define INSTALL_FUNCTIONS_H
#include <filesystem>
#include <config.h>
namespace MT{

void write_brightness_file_path(Config &config);
void write_actual_brightness_file_path(Config &config);
void write_max_brightness_file_path(Config &config);
void write_display_state_file_path(Config &config);
[[nodiscard]] bool folder_exists(const std::string & path);
void create_init_script();
std::string init_script();
void clear_innstall(const MT::Config &config);
}//namespace MT
#endif // INSTALL_FUNCTIONS_H
