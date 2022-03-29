#include "html_paragraph_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_paragraph_element::html_paragraph_element()
        : html_element()
{
    local_name = "p";
    HTML_CONSTRUCTOR
}


auto html::elements::html_paragraph_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_paragraph_element>{isolate}
            .template ctor<>()
            .template inherit<html_element>()
            .auto_wrap_objects();
}
