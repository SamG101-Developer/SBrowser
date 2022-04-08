#pragma once
#ifndef SBROWSER_HTML_LABEL_ELEMENT_HPP
#define SBROWSER_HTML_LABEL_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/form_associated.hpp>

namespace html::elements {class html_form_element;}
namespace html::elements {class html_label_element;}


class html::elements::html_label_element
        : public html_element
        , public mixins::form_associated<html_label_element>
{
public constructors:
    html_label_element();

public js_properties:
    ext::html_property<ext::string, _T> html_for;
    ext::html_property<html_element*> control;
    ext::html_property<html_form_element*> form;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private accessors:
    auto set_html_for(const ext::string& val) -> void;
    auto get_form() const -> html_form_element*;
};


#endif //SBROWSER_HTML_LABEL_ELEMENT_HPP
