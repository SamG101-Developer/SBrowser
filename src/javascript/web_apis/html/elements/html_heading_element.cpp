#include "html_heading_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_heading_element::html_heading_element() = default;


auto html::elements::html_heading_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_heading_element>{isolate}
            .ctor<>()
            .inherit<html_paragraph_element>()
            .auto_wrap_objects();
}
