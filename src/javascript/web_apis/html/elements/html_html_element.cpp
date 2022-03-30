#include "html_html_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_html_element::html_html_element()
        : html_element{}
{
    // initialize html constructor with boilerplate code
    HTML_CONSTRUCTOR
}


auto html::elements::html_html_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_html_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
