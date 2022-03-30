#include "html_div_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_div_element::html_div_element()
        : html_element{}
{
    local_name = "div";
    HTML_CONSTRUCTOR
}


auto html::elements::html_div_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_div_element>{isolate}
            .template ctor<>()
            .template inherit<html_element>()
            .auto_wrap_objects();
}
