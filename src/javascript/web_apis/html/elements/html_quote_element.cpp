#include "html_quote_element.hpp"


html::elements::html_quote_element::html_quote_element() = default;


auto html::elements::html_quote_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_quote_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("cite", &html_quote_element::cite)
            .auto_wrap_objects();
}
