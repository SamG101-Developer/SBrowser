#pragma once
#ifndef SBROWSER_HTML_TITLE_ELEMENT_HPP
#define SBROWSER_HTML_TITLE_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_title_element;}


class html::elements::html_title_element : public html_element
{
public constructors:
    html_title_element();

public js_properties:
    ext::html_property<ext::string, _T> text;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private accessors:
    auto get_text() const -> ext::string;
    auto set_text(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_TITLE_ELEMENT_HPP
