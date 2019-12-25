# Display_Brightness

Automatically adjust brightness of laptop display using webcam frame shots.
Designed for those «lucky ones» who either don’t have light sensor or light sensor isn’t detectable by Ubuntu.
## Install
1. Download three binaries (dispbr, Install and Uninstall) from release from release tab.
2. Open Terminal and run Install executable with super_user privileges.
   By default service is installed in /opt folder. You can specify another folder by providing it as an argument.
    ```
    Install install_path
    ```
   Service depends on four system files:
   
   max_brightness - system file to know the maximum possible brightness value;
   
   brightness - system file is used to change display brightness;
   
   actual_brightness - is used by inotify routine to catch brightness change not invoked by the service (e.g. from keyboard);
   
   dpms - is to track the display is on or off;
   
   Installation process will try to find those files, but in some cases user will have to specify them explicitly.    
   Appropriate message will be displayed.
   
## Uninstall
   Run Uninstall executable from installation directory.
   
### Usage
dispbr [COMMANDS]... [OPTIONAL ARGS]...

Commands:
    help            displays this message.
    start           start service with default values.
    stop            stops service.
    set brf [PATH]  set path to brightness file to change display brightness.
                    NOTE: Should be used only if appropriate message was displayed upon install.
    set abrf [PATH] set path to actual_brightness file to track display brightness changes.
                    NOTE: Should be used only if appropriate message was displayed upon install.
    By default service searches for appropriate files in /sys/class/backlight/[some folder]
    incbr [VALUE]   increase default brightness values for specified percent.
                    [VALUE] is an integer with range [1...10].
    decbr [VALUE]   decrease default brightness values for specified percent.
                    [VALUE] is an integer with range [1...10].
    status          displays service status [running] [running-sleep] [stopped]
                    [running-sleep] is a state when service was interrupted by
                    change of display brightness by a user (via keyboard) and
                    time interval was set to 150 min.
