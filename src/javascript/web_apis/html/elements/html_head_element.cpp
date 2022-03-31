#include "html_head_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_head_element::html_head_element() = default;


auto html::elements::html_head_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_head_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
