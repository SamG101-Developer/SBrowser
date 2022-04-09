#pragma once
#ifndef SBROWSER_HTML_HEAD_ELEMENT_HPP
#define SBROWSER_HTML_HEAD_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_head_element;}


class html::elements::html_head_element : public html_element
{
public constructors:
    html_head_element();

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_HEAD_ELEMENT_HPP
