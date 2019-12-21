#include "user_input_parser.h"
#include <cstring>

int MT::parse_user_input(const char *input)
{
    if (strcmp(input, "help") == 0){
        return 1;
    }
    if (strcmp(input, "start") == 0){
        return 2;
    }
    if (strcmp(input, "stop") == 0){
        return 3;
    }
    if (strcmp(input, "set_brf") == 0){
        return 4;
    }
    if (strcmp(input, "set_abrf") == 0){
        return 5;
    }
    if (strcmp(input, "incbr") == 0){
        return 6;
    }
    if (strcmp(input, "decbr") == 0){
        return 7;
    }
    if (strcmp(input, "status") == 0){
        return 8;
    }
    return 0;
}
