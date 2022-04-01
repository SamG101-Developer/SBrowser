#include "html_li_element.hpp"


html::elements::html_li_element::html_li_element() = default;


auto html::elements::html_li_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    v8pp::class_<html_li_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("value", &html_li_element::value)
            .auto_wrap_objects();
}
