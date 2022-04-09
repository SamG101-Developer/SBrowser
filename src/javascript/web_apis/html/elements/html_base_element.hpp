#pragma once
#ifndef SBROWSER_HTML_BASE_ELEMENT_HPP
#define SBROWSER_HTML_BASE_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_base_element;}


class html::elements::html_base_element : public html_element
{
public constructors:
    html_base_element();

public js_properties:
    ext::html_property<ext::string, _F> href;
    ext::html_property<ext::string, _F> target;

private accessors:
    auto get_href() const -> ext::string;

};


#endif //SBROWSER_HTML_BASE_ELEMENT_HPP
