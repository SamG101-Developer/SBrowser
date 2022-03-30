#pragma once
#ifndef SBROWSER_HTML_FIELD_SET_ELEMENT_HPP
#define SBROWSER_HTML_FIELD_SET_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_form_element;}
namespace html::elements {class html_field_set_element;}


class html::elements::html_field_set_element
        : public html_element
        , public mixins::validatable
{
public constructors:
    html_field_set_element();

public js_properties:
    ext::html_property<bool, _T> disabled;
    ext::html_property<html_form_element*> form;
    ext::html_property<ext::string, _T> name;
    ext::html_property<ext::string> type;
    ext::html_property<ext::vector<dom::nodes::element*>*> elements;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_FIELD_SET_ELEMENT_HPP
