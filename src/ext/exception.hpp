#ifndef SBROWSER_EXCEPTION_HPP
#define SBROWSER_EXCEPTION_HPP

#include <exception>

namespace ext {class impossible_exception;}
namespace ext {class invalid_enum_enumerator_exception;}
namespace ext {class invalid_switch_case_exception;}


// base class for impossible exceptions
class ext::impossible_exception : public std::runtime_error {};

// common exceptions thrown in situations that shouldn't ever happen
class ext::invalid_enum_enumerator_exception : public impossible_exception {};
class ext::invalid_switch_case_exception : public impossible_exception {};

#endif //SBROWSER_EXCEPTION_HPP
