#include "html_details_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_details_element::html_details_element() = default;

auto html::elements::html_details_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_details_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("open", &html_details_element::open)
            .auto_wrap_objects();
}
