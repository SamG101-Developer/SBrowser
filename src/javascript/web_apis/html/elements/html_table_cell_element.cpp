#include "html_table_cell_element.hpp"

#include <html/elements/html_table_row_element.hpp>


html::elements::html_table_cell_element::html_table_cell_element()
        : html_element()
{
    1
    scope.constrain_values({"row", "col", "rowgroup", "colgroup", "auto"});
    col_span.clamp_values(1, 1000);
    row_span.clamp_values(0, 65534);

    // set the custom accessor methods
    cell_index.getter = [this] {return get_cell_index();};

    // set the property's values
    col_span << 1UL;
    row_span << 1UL;
}


auto html::elements::html_table_cell_element::get_cell_index()
        -> long
{
    // get the index of this cell in the parent's cell list (if the parent is a HTMLTableRowElement element)
    if (auto* table_row_parent = ext::property_dynamic_cast<html_table_row_element*>(parent_element))
        return (long)table_row_parent->cells->find(this);

    // otherwise, return -1
    return -1;
}


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
