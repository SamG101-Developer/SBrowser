#include "html_picture_element.hpp"


html::elements::html_picture_element::html_picture_element() = default;


auto html::elements::html_picture_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_picture_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
