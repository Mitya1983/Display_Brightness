#include "error_handler.h"

std::string MT::error_message(MT::Error error)
{
    switch (error) {
    case MT::Error::EmptyArgument:{
        return "No arguments were provided.\nUse \'help\' for more information";
        break;
    }
    case MT::Error::InvalidArgument:{
        return "Invalid argument provided.\nUse \'help\' for more information";
    }
    case MT::Error::ValueWasntProvided:{
        return "Value for command wasn't provided.\nUse \'help\' for more information";
    }
    case MT::Error::ValueOutOfRange:{
        return "Value for command is out of range.\nUse \'help\' for more information";
    }
    }
}
