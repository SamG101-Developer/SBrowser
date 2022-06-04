#include "html_olist_element.hpp"


html::elements::html_olist_element::html_olist_element()
{
    // set the property's values
    start << 1l;
}


auto html::elements::html_olist_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_olist_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("reversed", &html_olist_element::reversed)
            .var("start", &html_olist_element::start)
            .var("type", &html_olist_element::type)
            .auto_wrap_objects();
}
