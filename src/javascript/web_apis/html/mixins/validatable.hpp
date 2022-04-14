#pragma once
#ifndef SBROWSER_VALIDATABLE_HPP
#define SBROWSER_VALIDATABLE_HPP

#include <ext/html_property.hpp>
#include <dom_object.hpp>

namespace html::mixins {template <typename T> class validatable;}


template <typename T>
class html::mixins::validatable : public virtual dom_object
{
public constructors:
    validatable();

public js_methods:
    auto check_validity() -> bool;
    auto report_validity() -> bool;
    auto set_custom_validity(const ext::string& error) -> void;

public js_properties:
    ext::html_property<bool, _F> will_validate;
    ext::html_property<ext::string, _F> validation_message;
    ext::html_property<ext::string, _F> validity;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    ext::string m_custom_validation_message;
};


#endif //SBROWSER_VALIDATABLE_HPP
