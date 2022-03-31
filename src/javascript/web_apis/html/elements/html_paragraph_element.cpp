#include "html_paragraph_element.hpp"


html::elements::html_paragraph_element::html_paragraph_element() = default;


auto html::elements::html_paragraph_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_paragraph_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
