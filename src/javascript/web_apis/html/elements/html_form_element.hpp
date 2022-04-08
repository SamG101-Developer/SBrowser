#pragma once
#ifndef SBROWSER_HTML_FORM_ELEMENT_HPP
#define SBROWSER_HTML_FORM_ELEMENT_HPP

#include <ext/listlike.hpp>
#include <html/elements/html_element.hpp>
#include <html/mixins/targetable.hpp>
#include <html/mixins/validatable.hpp>

namespace html::elements {class html_form_element;}


class html::elements::html_form_element
        : public html_element
        , public mixins::targetable<html_form_element>
        , public mixins::validatable<html_form_element>
        , public ext::listlike<dom::nodes::element*>
{
public constructors:
    html_form_element();

public js_methods:
    auto submit() -> void;
    auto reset() -> void;
    auto requestSubmit(html_element* submitter) -> void;

public js_properties:
    ext::html_property<ext::string, _T> acceptCharset;
    ext::html_property<ext::string, _T> action;
    ext::html_property<ext::string, _T> autocomplete;
    ext::html_property<ext::string, _T> enctype;
    ext::html_property<ext::string, _T> encoding;
    ext::html_property<ext::string, _T> method;
    ext::html_property<ext::string, _T> name;
    ext::html_property<bool, _T> noValidate;

    ext::html_property<ext::vector<html_form_element*>*> elements;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private cpp_properties:
    bool m_locked_for_reset = false;
    ext::map<ext::string, dom::nodes::element*> m_past_names_map;
};


#endif //SBROWSER_HTML_FORM_ELEMENT_HPP
