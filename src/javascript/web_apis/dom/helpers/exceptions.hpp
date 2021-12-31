#pragma once
#ifndef SBROWSER_EXCEPTIONS_HPP
#define SBROWSER_EXCEPTIONS_HPP

#define empty_conditional [](){return true;}

#include <functional>
#include <ext/iterables/string.hpp>
#include <dom/other/dom_exception.hpp>

namespace dom::helpers {struct exceptions;}


struct dom::helpers::exceptions {
    using exception_condiditional = std::function<bool()>&&;
    using v8_primitive_error_t = std::function<v8::Local<v8::Value>(v8::Local<v8::String>)>;

    static void throw_v8_exception(ext::cstring& exception_message, exception_type exception_type, exception_condiditional conditional = empty_conditional);
    static void throw_v8_exception(ext::cstring& exception_message, v8_primitive_error_t&& exception_type, exception_condiditional conditional = empty_conditional);
};





#endif //SBROWSER_EXCEPTIONS_HPP
