#ifndef INSTALL_FUNCTIONS_H
#define INSTALL_FUNCTIONS_H
#include <filesystem>
namespace MT{

/*
1. Create install directory
2. Search for brightness file
    if found
        write path to brightness_file_path file
    if not
        display message that user should specify brightness file path
3. Search for actual_brightness file
    if found
        write path to actual_brightness_file_path file
    if not
        display message that user should specify brightness file path
4. Search for max_brightness file
    if found
        write path to actual_brightness_file_path file
    if not
        display message that user should specify brightness file path
5. Search for display state file
6. Create brightness_values file
7. Create time_interval file
8. Create pid file with zero value
9. Create status file with stopped value
10. Copy executable to install directory
11. Check if /lib/systemd/system-sleep folder exists
    if found
        create invoke_script file in folder
    if not
        display message that user should specify folder for sleep start and stop trigger
12. Check if /lib/systemd/system-shutdown folder exists
    if found
        create invoke_script file in folder
    if not
        display message that user should specify folder for shutdown start and stop trigger
13. Implement lockscreen start and stop trigger
14. Create simlink in /usr/bin folder
*/

[[nodiscard]] bool write_brightness_file_path();
[[nodiscard]] bool write_actual_brightness_file_path();
[[nodiscard]] bool write_max_brightness_file_path();
[[nodiscard]] bool write_display_state_file_path();
[[nodiscard]] bool folder_exists(const std::string & path);
std::string folder_search(const std::string &folder_name);
void create_invoke_script(const std::string &path);
std::string invoke_script();

void clear_install();

}//namespace MT
#endif // INSTALL_FUNCTIONS_H
