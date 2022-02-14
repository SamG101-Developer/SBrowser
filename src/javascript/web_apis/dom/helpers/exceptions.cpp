#include "exceptions.hpp"

#include <v8.h>


void
dom::helpers::exceptions::throw_v8_exception(
        ext::cstring& exception_message,
        exception_type exception_type,
        exception_condiditional conditional)
{
    // if the condition is met, then throw the error in javascript
    if (conditional())
        v8::Isolate::GetCurrent()->ThrowError(exception_message);
}


void dom::helpers::exceptions::throw_v8_exception(
        ext::cstring& exception_message,
        v8_primitive_error_t&& exception_type,
        exception_condiditional conditional)
{
    // if the condition is met, then throw the error in javascript
    if (conditional())
        v8::Isolate::GetCurrent()->ThrowException(exception_type(exception_message));
}
