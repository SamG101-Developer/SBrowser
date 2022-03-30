#include "html_dlist_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_dlist_element::html_dlist_element()
        : html_element{}
{
    local_name = "dl";
    HTML_CONSTRUCTOR
}


auto html::elements::html_dlist_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_dlist_element>{isolate}
            .template ctor<>()
            .template inherit<html_element>()
            .auto_wrap_objects();
}
