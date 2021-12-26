#include "exceptions.hpp"

#include <v8.h>


void
dom::helpers::exceptions::throw_v8_exception(
        ext::cstring& exception_message,
        exception_type exception_type,
        exception_condiditional conditional) {

    if (conditional())
        v8::Isolate::GetCurrent()->ThrowError(exception_message.to_string_v8());
}


void dom::helpers::exceptions::throw_v8_exception(
        ext::cstring& exception_message,
        v8_primitive_error_t&& exception_type,
        exception_condiditional conditional) {

    if (conditional())
        v8::Isolate::GetCurrent()->ThrowError(exception_type(exception_message.to_string_v8()));
}
