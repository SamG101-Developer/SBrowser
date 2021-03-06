#pragma once
#ifndef SBROWSER_HTML_QUOTE_ELEMENT_HPP
#define SBROWSER_HTML_QUOTE_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_quote_element;}


class html::elements::html_quote_element : public html_element
{
public constructors:
    html_quote_element();

public js_properties:
    ext::html_property<ext::string, _T> cite;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_QUOTE_ELEMENT_HPP
