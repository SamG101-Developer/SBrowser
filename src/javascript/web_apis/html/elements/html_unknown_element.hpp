#pragma once
#ifndef SBROWSER_HTML_UNKNOWN_ELEMENT_HPP
#define SBROWSER_HTML_UNKNOWN_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_unknown_element;}


class html::elements::html_unknown_element : public html_element
{
public:
    html_unknown_element();
};


#endif //SBROWSER_HTML_UNKNOWN_ELEMENT_HPP
