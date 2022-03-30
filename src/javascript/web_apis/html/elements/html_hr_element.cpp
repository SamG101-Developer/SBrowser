#include "html_hr_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_hr_element::html_hr_element()
        : html_element{}
{
    // initialize html constructor with boilerplate code
    HTML_CONSTRUCTOR
}


auto html::elements::html_hr_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_hr_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
