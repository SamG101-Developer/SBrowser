#include "html_span_element.hpp"


html::elements::html_span_element::html_span_element() = default;


auto html::elements::html_span_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_span_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
