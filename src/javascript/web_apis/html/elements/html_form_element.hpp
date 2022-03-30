#pragma once
#ifndef SBROWSER_HTML_FORM_ELEMENT_HPP
#define SBROWSER_HTML_FORM_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_form_element;}


class html::elements::html_form_element
        : public html_element
        , public mixins::validatable
{
public constructors:
    html_form_element();

public methods:
    auto submit() -> void;
    auto reset() -> void;
    auto requestSubmit(html_element* submitter) -> void;

public properties:
    ext::html_property<ext::string, _T> acceptCharset;
    ext::html_property<ext::string, _T> action;
    ext::html_property<ext::string, _T> autocomplete;
    ext::html_property<ext::string, _T> enctype;
    ext::html_property<ext::string, _T> encoding;
    ext::html_property<ext::string, _T> method;
    ext::html_property<ext::string, _T> name;
    ext::html_property<ext::string, _T> target;
    ext::html_property<ext::string, _T> rel;
    ext::html_property<bool, _T> noValidate;

    ext::html_property<ext::vector<ext::string>*> relList;
    ext::html_property<ext::vector<html_form_element*>*> elements;

public internal_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

protected internal_properties:
    bool m_locked_for_reset = false;
};


#endif //SBROWSER_HTML_FORM_ELEMENT_HPP
