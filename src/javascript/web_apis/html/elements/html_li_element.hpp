#pragma once
#ifndef SBROWSER_HTML_LI_ELEMENT_HPP
#define SBROWSER_HTML_LI_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_li_element;}


class html::elements::html_li_element : public html_element
{
public constructors:
    html_li_element();

public js_properties:
    ext::html_property<long, _T> value;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_LI_ELEMENT_HPP
