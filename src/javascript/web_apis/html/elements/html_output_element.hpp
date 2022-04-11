#pragma once
#ifndef SBROWSER_HTML_OUTPUT_ELEMENT_HPP
#define SBROWSER_HTML_OUTPUT_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/form_associated.hpp>
#include <html/mixins/labelable.hpp>
#include <html/mixins/validatable.hpp>

namespace html::elements {class html_output_element;}


class html::elements::html_output_element
        : public html_element
        , public mixins::form_associated<html_output_element>
        , public mixins::labelable<html_output_element>
        , public mixins::validatable<html_output_element>
{
public constructors:
    html_output_element();

public js_properties:
    ext::html_property<ext::string_vector*, _F> html_for;
    ext::html_property<ext::string, _T> default_value;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private cpp_properties:
    ext::string m_default_value_override;

private accessors:
    auto get_default_value() const -> ext::string;
    auto get_value() const -> ext::string;

    auto set_default_value(const ext::string& val) -> void;
    auto set_value(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_OUTPUT_ELEMENT_HPP
