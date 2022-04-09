#pragma once
#ifndef SBROWSER_HTML_OLIST_ELEMENT_HPP
#define SBROWSER_HTML_OLIST_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_olist_element;}


class html::elements::html_olist_element : public html_element
{
public constructors:
    html_olist_element();

public js_properties:
    ext::html_property<bool, _T> reversed;
    ext::html_property<long, _T> start;
    ext::html_property<ext::string, _T> type;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_OLIST_ELEMENT_HPP
