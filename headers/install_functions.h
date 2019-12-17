#ifndef INSTALL_FUNCTIONS_H
#define INSTALL_FUNCTIONS_H
#include <filesystem>
namespace MT{



[[nodiscard]] bool write_brightness_file_path();
[[nodiscard]] bool write_actual_brightness_file_path();
[[nodiscard]] bool folder_exists(const std::string & path);
std::string folder_search(const std::string &folder_name);
void create_invoke_script(const std::string &path);
std::string invoke_script();

void clear_install();

}//namespace MT
#endif // INSTALL_FUNCTIONS_H
