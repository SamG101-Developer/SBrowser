#pragma once
#ifndef SBROWSER_HTML_AREA_ELEMENT_HPP
#define SBROWSER_HTML_AREA_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_area_element;}


class html::elements::html_area_element
        : public html_element
        , public mixins::html_hyperlink_element_utils
{
public: constructors
    html_area_element();

public: properties
    ext::dom_property<ext::string> alt;
    ext::dom_property<ext::string> coords;
    ext::dom_property<ext::string> shape;
    ext::dom_property<ext::string> target;
    ext::dom_property<ext::string> download;
    ext::dom_property<ext::string> ping;
    ext::dom_property<ext::string> rel;
    ext::dom_property<ext::string> referrerPolicy;
    ext::dom_property<ext::vector<ext::string>*> relList;
};


#endif //SBROWSER_HTML_AREA_ELEMENT_HPP
