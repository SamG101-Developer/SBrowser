#pragma once
#ifndef SBROWSER_CUSTOM_HTML_ELEMENTS_HPP
#define SBROWSER_CUSTOM_HTML_ELEMENTS_HPP

#include <javascript/environment/realms.hpp>

#define HTML_CONSTRUCTOR
//    auto* registry = javascript::realms::current_global_object().get<html::internal::custom_element_registry*>(m_local_name); \
//    if (javascript::helpers::new_target() == javascript::helpers::active_function_object())                                   \
//    {                                                                                                                         \
//        auto* isolate = v8::Isolate::GetCurrent()                                                                             \
//        isolate->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8Literal(isolate, "Error")))                   \
//    }                     \
//    auto* definition = registry.filter_values([new_target](auto* entry){return entry->constructor == new_target;})            \
//    ext::string is_value; \
//    TODO

#include <ext/iterables/string.hpp>

namespace dom::nodes {class element;}
namespace html {
    namespace elements {class html_element;}
    namespace helpers {struct custom_html_elements;}
}


struct html::helpers::custom_html_elements {
    static auto element_interface(ext::cstring& local_name, ext::cstring& namespace_) -> elements::html_element*;

    static auto a() -> int {
        v8::Isolate::GetCurrent()->ThrowException(v8::Exception::TypeError(v8::String::NewFromUtf8Literal(v8::Isolate::GetCurrent(), "Error")))
    }
};


#endif //SBROWSER_CUSTOM_HTML_ELEMENTS_HPP
