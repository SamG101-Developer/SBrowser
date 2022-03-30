#include "html_details_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_details_element::html_details_element()
        : html_element()
{
    local_name = "details";
    HTML_CONSTRUCTOR
}


auto html::elements::html_details_element::v8(
        v8::Isolate* isolate) const
        -> ext::any {

    return v8pp::class_<html_details_element>{isolate}
            .template ctor<>()
            .template inherit<html_element>()
            .template var("open", &html_details_element::open)
            .auto_wrap_objects();
}
