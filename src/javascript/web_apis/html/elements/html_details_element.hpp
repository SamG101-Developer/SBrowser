#pragma once
#ifndef SBROWSER_HTML_DETAILS_ELEMENT_HPP
#define SBROWSER_HTML_DETAILS_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_details_element;}


class html::elements::html_details_element : public html_element
{
public constructors:
    html_details_element();

public js_properties:
    ext::html_property<bool, _T> open;

public cpp_methods:
    auto qt() const -> QWidget* override;
    auto v8(v8::Isolate* isolate) const -> ext::any override;

public accessors:
    auto set_open(bool val) -> void;
};


#endif //SBROWSER_HTML_DETAILS_ELEMENT_HPP
