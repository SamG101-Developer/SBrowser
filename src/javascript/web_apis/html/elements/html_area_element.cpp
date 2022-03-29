#include "html_area_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_area_element::html_area_element()
        : html_element{}
        , mixins::html_hyperlink_element_utils{}
{
    // set the properties
    relList << new ext::vector<ext::string>{};
    local_name << "area";

    HTML_CONSTRUCTOR
}


auto html::elements::html_area_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_area_element>{isolate}
            .template ctor<>()
            .template inherit<html_area_element>()
//            .template inherit<mixins::html_hyperlink_element_utils>()
            .template var("alt", html_area_element::alt)
            .template var("coords", html_area_element::coords)
            .template var("shape", html_area_element::shape)
            .template var("target", html_area_element::target)
            .template var("download", html_area_element::download)
            .template var("ping", html_area_element::ping)
            .template var("rel", html_area_element::rel)
            .template var("referrerPolicy", html_area_element::referrerPolicy)
            .template var("relList", html_area_element::relList)
            .auto_wrap_objects();
}
