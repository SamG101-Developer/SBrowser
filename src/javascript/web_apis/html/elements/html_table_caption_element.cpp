#include "html_table_caption_element.hpp"


html::elements::html_table_caption_element::html_table_caption_element() = default;


auto html::elements::html_table_caption_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_table_caption_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .auto_wrap_objects();
}
