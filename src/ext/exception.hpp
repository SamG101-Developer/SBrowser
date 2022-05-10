#ifndef SBROWSER_EXCEPTION_HPP
#define SBROWSER_EXCEPTION_HPP

namespace ext {class impossible_exception;}
namespace ext {class invalid_enum_enumerator_exception;}
namespace ext {class invalid_switch_case_exception;}
namespace ext {class type_error;}

#include <stdexcept>
#include <ext/string.hpp>


// base class for impossible exceptions
class ext::impossible_exception : public std::runtime_error
{
public:
    using _Mybase = std::runtime_error;
    using std::runtime_error::runtime_error;
};


// common exceptions thrown in situations that shouldn't ever happen
class ext::invalid_enum_enumerator_exception : public impossible_exception
{
public:
    using _Mybase = impossible_exception;
    using impossible_exception::impossible_exception;
};


class ext::invalid_switch_case_exception : public impossible_exception
{
public:
    using _Mybase = impossible_exception;
    using impossible_exception::impossible_exception;
};


// common exceptions that should be customized
class ext::type_error : public std::runtime_error
{
public:
    using _Mybase = std::runtime_error;
    using std::runtime_error::runtime_error;
};

#endif //SBROWSER_EXCEPTION_HPP
