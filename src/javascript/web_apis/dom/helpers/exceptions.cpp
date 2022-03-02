#include "exceptions.hpp"

#include <v8.h>


auto dom::helpers::exceptions::throw_v8_exception(
        ext::cstring& exception_message,
        const v8_custom_error_t& exception_type,
        const exception_condiditional_t& conditional)
        -> void
{
    // if the condition is met, then throw the error in javascript
    if (conditional())
        v8::Isolate::GetCurrent()->ThrowError(exception_message.to_v8_string());
}


auto dom::helpers::exceptions::throw_v8_exception(
        ext::cstring& exception_message,
        const v8_primitive_error_t& exception_type,
        const exception_condiditional_t& conditional)
        -> void
{
    // if the condition is met, then throw the error in javascript
    if (conditional())
        v8::Isolate::GetCurrent()->ThrowException(exception_type(exception_message.to_v8_string()));
}
