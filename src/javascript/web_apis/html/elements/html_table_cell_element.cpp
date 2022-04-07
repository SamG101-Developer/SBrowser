#include "html_table_cell_element.hpp"


html::elements::html_table_cell_element::html_table_cell_element() = default;


auto html::elements::html_table_cell_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_table_cell_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("col_span", &html_table_cell_element::col_span)
            .var("row_span", &html_table_cell_element::row_span)
            .var("headers", &html_table_cell_element::headers)
            .var("cell_index", &html_table_cell_element::cell_index)
            .var("scope", &html_table_cell_element::scope)
            .var("abbr", &html_table_cell_element::abbr)
            .auto_wrap_objects();
}
