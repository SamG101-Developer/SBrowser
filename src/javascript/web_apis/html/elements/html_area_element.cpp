#include "html_area_element.hpp"


html::elements::html_area_element::html_area_element()
        : html_element{}
        , mixins::targetable<html_area_element>{}
        , mixins::html_hyperlink_element_utils{}
{
    // set the properties
    relList << new ext::string_vector{};
}


auto html::elements::html_area_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_area_element>{isolate}
            .ctor<>()
            .inherit<html_area_element>()
            .inherit<mixins::targetable<html_area_element>>()
//            .inherit<mixins::html_hyperlink_element_utils>()
            .var("alt", html_area_element::alt)
            .var("coords", html_area_element::coords)
            .var("shape", html_area_element::shape)
            .var("download", html_area_element::download)
            .var("ping", html_area_element::ping)
            .var("rel", html_area_element::rel)
            .var("referrerPolicy", html_area_element::referrerPolicy)
            .var("relList", html_area_element::relList)
            .auto_wrap_objects();
}
