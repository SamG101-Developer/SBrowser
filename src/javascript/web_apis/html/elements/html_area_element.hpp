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
    ext::dom_property<ext::string, _F> alt;
    ext::dom_property<ext::string, _F> coords;
    ext::dom_property<ext::string, _F> shape;
    ext::dom_property<ext::string, _F> target;
    ext::dom_property<ext::string, _F> download;
    ext::dom_property<ext::string, _F> ping;
    ext::dom_property<ext::string, _F> rel;
    ext::dom_property<ext::string, _F> referrerPolicy;
    ext::dom_property<ext::vector<ext::string>*, _F> relList;
};


#endif //SBROWSER_HTML_AREA_ELEMENT_HPP
