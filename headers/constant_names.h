#ifndef CONSTANT_NAMES_H
#define CONSTANT_NAMES_H
#include <string>
namespace MT {

namespace Constants {

const std::string executable_name = "dispbr";
const std::string status_file_name = "status";
const std::string log_file_name = "log.txt";
const std::string pid_file_name = "pid";
const std::string brightness_values_file_name = "brightness_values";

const std::string help_message = "Usage: dispbr [COMMANDS]... [OPTIONAL ARGS]...\n\
Automatically adjust brightness of laptop display using webcam frame shots.\n\
Designed for those «lucky ones» who either don’t have light sensor or light sensor isn’t detectable by Ubuntu.\n\n\
Commands:\n\n\
\thelp\t\tdisplays this message.\n\n\
\tstart\t\tstart service with default values.\n\n\
\tstop\t\tstops service.\n\n\
\tset_brf [PATH]\tset path to brightness file to change display brightness.\n\
\t\t\tNOTE: Should be used only if appropriate message was displayed upon install.\n\n\
\tset_abrf [PATH]\tset path to actual_brightness file to track display brightness changes.\n\
\t\t\tNOTE: Should be used only if appropriate message was displayed upon install.\n\n\
\t\t\tNOTE: By default service searches for appropriate files in /sys/class/backlight/[some folder]\n\n\
\tincbr [VALUE]\tincrease default brightness values for specified percent.\n\
\t\t\t[VALUE] is an integer with range [1...10].\n\n\
\tdecbr [VALUE]\tdecrease default brightness values for specified percent.\n\
\t\t\t[VALUE] is an integer with range [1...10].\n\n\
\tstatus\t\tdisplays service status [running] [running-sleep] [stopped]\n\
\t\t\t[sleep] is a state when service was interrupted by\n\
\t\t\tchange of display brightness by a user (via keyboard) and\n\
\t\t\ttime interval was set to 150 min.";

} // namespace Constants

} // namespace MT

#endif // CONSTANT_NAMES_H
