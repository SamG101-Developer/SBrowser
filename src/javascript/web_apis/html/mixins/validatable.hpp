#pragma once
#ifndef SBROWSER_VALIDATABLE_HPP
#define SBROWSER_VALIDATABLE_HPP

#include <ext/html_property.hpp>
#include <dom_object.hpp>

namespace html::mixins {template <typename T> class validatable;}


template <typename T>
class html::mixins::validatable : virtual public dom_object
{
public constructors:
    validatable();

public js_methods:
    auto check_validity() -> bool;
    auto report_validity() -> bool;
    auto set_custom_validity() -> void;

public js_properties:
    ext::html_property<bool> will_validate;
    ext::html_property<ext::string> validation_message;
    ext::html_property<ext::string> validity;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_VALIDATABLE_HPP
