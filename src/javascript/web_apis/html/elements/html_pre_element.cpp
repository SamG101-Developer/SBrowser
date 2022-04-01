#include "html_pre_element.hpp"


html::elements::html_pre_element::html_pre_element() = default;


auto html::elements::html_pre_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_pre_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
