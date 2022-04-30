#pragma once
#ifndef SBROWSER_HTML_AREA_ELEMENT_HPP
#define SBROWSER_HTML_AREA_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/targetable.hpp>
#include <html/mixins/html_hyperlink_element_utils.hpp>

namespace html::elements {class html_area_element;}


class html::elements::html_area_element
        : public html_element
        , public mixins::targetable<html_area_element>
        , public mixins::html_hyperlink_element_utils
{
public constructors:
    html_area_element();

public js_properties:
    ext::html_property<ext::string, _F> alt;
    ext::html_property<ext::string, _F> coords;
    ext::html_property<ext::string, _F> shape;
    ext::html_property<ext::string, _F> download;
    ext::html_property<ext::string, _F> ping;
    ext::html_property<ext::string, _F> referrerPolicy;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_AREA_ELEMENT_HPP
