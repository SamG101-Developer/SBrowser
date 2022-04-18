#ifndef SBROWSER_EXCEPTION_HPP
#define SBROWSER_EXCEPTION_HPP

#include <exception>

#define INVALID_ENUM(argument) std::runtime_error{"Invalid enum value (impossible)"}
#define INVALID_SWITCH(argument) std::runtime_error{"Invalid switch value (impossible)"}

#endif //SBROWSER_EXCEPTION_HPP
