#pragma once
#ifndef SBROWSER_HTML_DATA_ELEMENT_HPP
#define SBROWSER_HTML_DATA_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_data_element;}

class html::elements::html_data_element : public html_element
{
public constructors:
    html_data_element();

public properties:
    ext::html_property<ext::string, _T> value;

public internal_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_DATA_ELEMENT_HPP