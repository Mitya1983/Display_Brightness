#include <iostream>
#include <thread>
#include <log.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <filesystem>
#include <fstream>
#include "error_handler.h"
#include "function_to_fork.h"
#include "install_functions.h"
#include "user_input_parser.h"
#include "constant_names.h"
#include "config.h"
int main(int argc, char**argv)
{

    switch (argc) {
    case 1: {
        std::cout << MT::error_message(MT::Error::EmptyArgument) << std::endl;
        break;
    }
    case 2: {
        std::string to_log = "Argument ";
        to_log += argv[1];
        to_log += " received.";
        MT::Log::log().writeToLog(to_log);
        int parse = MT::parse_user_input(argv[1]);
        switch(parse){
        case 0:{
            std::cout << MT::error_message(MT::Error::InvalidArgument) << std::endl;
            MT::Log::log().writeToLog(MT::error_message(MT::Error::InvalidArgument));
            break;
        }
        case 1:{
            std::cout << MT::Constants::help_message << std::endl;
            break;
        }
        case 2:{
            if (MT::read_status_from_file() == "running"){
                std::cout << "Service is already running" << std::endl;
            }
            else{
                std::unique_ptr<MT::Config> config = std::make_unique<MT::Config>();
                try {
                    config->load_config();
                } catch (std::fstream::failure &e) {
                    MT::Log::log().writeToLog(e.what());
                    std::cout << e.what() << std::endl;
                    exit(EXIT_FAILURE);
                }
                pid_t child = fork();
                if (child < 0){
                    std::string msg = "fork() error: ";
                    msg += std::error_code(errno, std::generic_category()).message();
                    std::cout << msg << std::endl;
                    exit(EXIT_FAILURE);
                }
                if (child == 0){
                    MT::brightness_adjust(*config);
                }
            }
            break;
        }
        case 3:{
            pid_t child_pid = MT::read_pid_from_file();
            int kill_status = kill(child_pid, SIGQUIT);
            if (kill_status < 0){
                std::string msg = "kill() error: ";
                msg += std::error_code(errno, std::generic_category()).message();
                std::cout << msg << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        }
        case 4:{
            [[fallthrough]];
        }
        case 5:{
            [[fallthrough]];
        }
        case 6:{
            [[fallthrough]];
        }
        case 7:{
            std::cout << MT::error_message(MT::Error::ValueWasntProvided) << std::endl;
            MT::Log::log().writeToLog(MT::error_message(MT::Error::ValueWasntProvided));
            break;
        }
        case 8:{
            std::cout << "Service is " << MT::read_status_from_file() << std::endl;
            break;
        }
        }
        break;
    }
    case 3:{
        std::string to_log = "Argument ";
        to_log += argv[1];
        to_log += " received.";
        MT::Log::log().writeToLog(to_log);
        int parse = MT::parse_user_input(argv[1]);
        switch(parse){
        case 0:{
            [[fallthrough]];
        }
        case 1:{
            [[fallthrough]];
        }
        case 3:{
            [[fallthrough]];
        }
        case 8:{
            std::cout << MT::error_message(MT::Error::InvalidArgument) << std::endl;
            MT::Log::log().writeToLog(MT::error_message(MT::Error::InvalidArgument));
            break;
        }
        case 4:{
            break;
        }
        case 5:{
            break;
        }
        case 6:{
            int value = std::stoi(argv[2]);
            if (value < 1 || value > 10){
                std::cout << MT::error_message(MT::Error::ValueOutOfRange) << std::endl;
                MT::Log::log().writeToLog(MT::error_message(MT::Error::ValueOutOfRange));
            }
            else{
                MT::change_brightness_values(value, MT::PlusOrMinus::Increase);
                pid_t child_pid = MT::read_pid_from_file();
                int kill_status = kill(child_pid, SIGUSR1);
                if (kill_status < 0){
                    std::string msg = "kill() error: ";
                    msg += std::error_code(errno, std::generic_category()).message();
                    std::cout << msg << std::endl;
                    MT::Log::log().writeToLog(msg);
                    MT::Log::log().writeToLog("Exiting with failure");
                    exit(EXIT_FAILURE);
                }
            }
            break;
        }
        case 7:{
            int value = std::stoi(argv[2]);
            if (value < 1 || value > 10){
                std::cout << MT::error_message(MT::Error::ValueOutOfRange) << std::endl;
                MT::Log::log().writeToLog(MT::error_message(MT::Error::ValueOutOfRange));
            }
            else{
                MT::change_brightness_values(value, MT::PlusOrMinus::Decrease);
                pid_t child_pid = MT::read_pid_from_file();
                int kill_status = kill(child_pid, SIGUSR1);
                if (kill_status < 0){
                    std::string msg = "kill() error: ";
                    msg += std::error_code(errno, std::generic_category()).message();
                    std::cout << msg << std::endl;
                    MT::Log::log().writeToLog(msg);
                    MT::Log::log().writeToLog("Exiting with failure");
                    exit(EXIT_FAILURE);
                }
            }
            break;
        }
        }
        break;
    }
    }
    return 0;
}
