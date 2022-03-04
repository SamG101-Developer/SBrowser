#pragma once
#ifndef SBROWSER_EXCEPTIONS_HPP
#define SBROWSER_EXCEPTIONS_HPP

#define null_exception_conditional [](){return true;}

#include <functional>

#include <dom/other/dom_exception.hpp>

namespace dom::helpers {struct exceptions;}


struct dom::helpers::exceptions
{
    using exception_condiditional_t = std::function<bool()>;
    using v8_primitive_error_t = std::function<v8::Local<v8::Value>(v8::Local<v8::String>)>;

    static auto throw_v8_exception(const ext::string& exception_message, const v8_custom_error_t&    exception_type, const exception_condiditional_t& conditional = null_exception_conditional) -> void;
    static auto throw_v8_exception(const ext::string& exception_message, const v8_primitive_error_t& exception_type, const exception_condiditional_t& conditional = null_exception_conditional) -> void;
};





#endif //SBROWSER_EXCEPTIONS_HPP
