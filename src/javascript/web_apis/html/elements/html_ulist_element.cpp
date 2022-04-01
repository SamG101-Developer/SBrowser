#include "html_ulist_element.hpp"


html::elements::html_ulist_element::html_ulist_element() = default;


auto html::elements::html_ulist_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_ulist_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
