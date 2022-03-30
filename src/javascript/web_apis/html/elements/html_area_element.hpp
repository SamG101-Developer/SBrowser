#pragma once
#ifndef SBROWSER_HTML_AREA_ELEMENT_HPP
#define SBROWSER_HTML_AREA_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_area_element;}


class html::elements::html_area_element
        : public html_element
        , public mixins::html_hyperlink_element_utils
{
public constructors:
    html_area_element();

public js_properties:
    ext::html_property<ext::string> alt;
    ext::html_property<ext::string> coords;
    ext::html_property<ext::string> shape;
    ext::html_property<ext::string> target;
    ext::html_property<ext::string> download;
    ext::html_property<ext::string> ping;
    ext::html_property<ext::string> rel;
    ext::html_property<ext::string> referrerPolicy;
    ext::html_property<ext::vector<ext::string>*> relList;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_AREA_ELEMENT_HPP
