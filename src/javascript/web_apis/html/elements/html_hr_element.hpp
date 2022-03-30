#pragma once
#ifndef SBROWSER_HTML_HR_ELEMENT_HPP
#define SBROWSER_HTML_HR_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_hr_element;}


class html::elements::html_hr_element : public html_element
{
public constructors:
    html_hr_element();

public internal_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_HR_ELEMENT_HPP
