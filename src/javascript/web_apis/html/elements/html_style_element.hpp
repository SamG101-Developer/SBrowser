#pragma once
#ifndef SBROWSER_HTML_STYLE_ELEMENT_HPP
#define SBROWSER_HTML_STYLE_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_style_element;}


class html::elements::html_style_element : public html_element
{
public constructors:
    html_style_element();

public js_properties:
    ext::html_property<ext::string, _T> media;
    ext::html_property<ext::string_vector*> blocking;
};


#endif //SBROWSER_HTML_STYLE_ELEMENT_HPP
