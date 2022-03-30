#pragma once
#ifndef SBROWSER_HTML_DETAILS_ELEMENT_HPP
#define SBROWSER_HTML_DETAILS_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_details_element;}


class html::elements::html_details_element : public html_element
{
public constructors:
    html_details_element();

public properties:
    ext::html_property<bool, _T> open;

public internal_methods:
    auto render() const -> QWidget* override;
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_DETAILS_ELEMENT_HPP
