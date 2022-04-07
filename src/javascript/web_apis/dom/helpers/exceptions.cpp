#include "exceptions.hpp"

#include <v8.h>


template <v8_custom_error_t exception_type>
auto dom::helpers::exceptions::throw_v8_exception(
        const ext::string& exception_message,
        const exception_condiditional_t& conditional)
        -> void
{
    // if the condition is met, then throw the error in javascript
    if (conditional())
        v8::Isolate::GetCurrent()->ThrowError(exception_message);
}


template <v8_primitive_error_t exception_type>
auto dom::helpers::exceptions::throw_v8_exception(
        const ext::string& exception_message,
        const exception_condiditional_t& conditional)
        -> void
{
    // if the condition is met, then throw the error in javascript
    v8::Local<v8::Value> v8_exception_constructor;

    // get the correct javascript primitive error type
    switch(exception_type)
    {
        case (V8_TYPE_ERROR): v8_exception_constructor = v8::Exception::TypeError(exception_message);
        case (V8_RANGE_ERROR): v8_exception_constructor = v8::Exception::RangeError(exception_message);
        case (V8_REFERENCE_ERROR): v8_exception_constructor = v8::Exception::ReferenceError(exception_message);
        case (V8_SYNTAX_ERROR): v8_exception_constructor = v8::Exception::SyntaxError(exception_message);
        case (V8_WASM_COMPILE_ERROR): v8_exception_constructor = v8::Exception::WasmCompileError(exception_message);
        case (V8_WASM_LINK_ERROR): v8_exception_constructor = v8::Exception::WasmLinkError(exception_message);
        case (V8_WASM_RUNTIME_ERROR): v8_exception_constructor = v8::Exception::WasmRuntimeError(exception_message);
    }

    if (conditional())
        v8::Isolate::GetCurrent()->ThrowException(v8_exception_constructor);
}
