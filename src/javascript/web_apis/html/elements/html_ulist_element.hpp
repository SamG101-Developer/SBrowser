#pragma once
#ifndef SBROWSER_HTML_ULIST_ELEMENT_HPP
#define SBROWSER_HTML_ULIST_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_ulist_element;};


class html::elements::html_ulist_element : public html_element
{
public constructors:
    html_ulist_element();

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_ULIST_ELEMENT_HPP
