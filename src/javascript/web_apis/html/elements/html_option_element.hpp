#pragma once
#ifndef SBROWSER_HTML_OPTION_ELEMENT_HPP
#define SBROWSER_HTML_OPTION_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/form_associated.hpp>

namespace html::elements {class html_option_element;}


class html::elements::html_option_element
        : public html_element
        , public mixins::form_associated<html_option_element>
{
public constructors:
    html_option_element();

public js_properties:
    ext::html_property<bool, _F> selected;

    ext::html_property<ext::string, _T> label;
    ext::html_property<ext::string, _T> value;
    ext::html_property<ext::string, _T> text;

    ext::html_property<ulong, _T> index;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    bool m_dirtiness_flag = false;

private accessors:
    auto get_disabled() const -> bool;
    auto get_label() const -> ext::string;
    auto get_text() const -> ext::string;
    auto get_index() const -> ulong;
    auto get_form() const -> html_form_element*;

    auto set_selected(bool val) -> void;
    auto set_text(const ext::string& val) -> void;

    auto del_selected() -> void;
};


#endif //SBROWSER_HTML_OPTION_ELEMENT_HPP
