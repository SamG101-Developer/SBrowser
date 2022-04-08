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
    ext::html_property<ext::string> alt;
    ext::html_property<ext::string> coords;
    ext::html_property<ext::string> shape;
    ext::html_property<ext::string> download;
    ext::html_property<ext::string> ping;
    ext::html_property<ext::string> referrerPolicy;

public cpp_methods:
    auto activation_behaviour(dom::events::event* event) -> void override;
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_AREA_ELEMENT_HPP
