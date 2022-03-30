#pragma once
#ifndef SBROWSER_HTML_HEADING_ELEMENT_HPP
#define SBROWSER_HTML_HEADING_ELEMENT_HPP

#include <html/elements/html_paragraph_element.hpp>

namespace html::elements {class html_heading_element;}


class html::elements::html_heading_element : public html_paragraph_element
{
public constructors:
    html_heading_element();

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_HEADING_ELEMENT_HPP
