#ifndef SBROWSER_EXCEPTION_HPP
#define SBROWSER_EXCEPTION_HPP

#include <exception>

#define INVALID_ARGUMENT(argument) std::invalid_argument{"Invalid value in the <> argument"}
#define INVALID_ENUM(argument) std::runtime_error{"IInvalid enum value (impossible)"}

#endif //SBROWSER_EXCEPTION_HPP
