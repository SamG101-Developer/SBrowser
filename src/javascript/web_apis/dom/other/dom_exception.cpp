#include "dom_exception.hpp"


dom::other::dom_exception::dom_exception(
        ext::cstring& message,
        exception_type type) {

    this->message = message;
    this->type = type;
}
