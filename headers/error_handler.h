#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H
#include <string>
namespace MT{

enum class Error
{
    EmptyArgument,
    InvalidArgument,
    ValueWasntProvided,
    ValueOutOfRange,

};

[[nodiscard]] std::string error_message(Error error);

}//namespace MT
#endif // ERROR_HANDLER_H
