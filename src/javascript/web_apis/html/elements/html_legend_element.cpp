#include "html_legend_element.hpp"


html::elements::html_legend_element::html_legend_element() = default;


auto html::elements::html_legend_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_legend_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
