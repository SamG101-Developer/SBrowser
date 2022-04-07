#include "html_table_col_element.hpp"


html::elements::html_table_col_element::html_table_col_element() = default;


auto html::elements::html_table_col_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_table_col_element>{isolate}
            .inherit<html_element>()
            .var("span", &html_table_col_element::span)
            .auto_wrap_objects();
}
