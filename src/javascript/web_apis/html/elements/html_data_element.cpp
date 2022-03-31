#include "html_data_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_data_element::html_data_element() = default;


auto html::elements::html_data_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_data_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("value", &html_data_element::value)
            .auto_wrap_objects();
}
