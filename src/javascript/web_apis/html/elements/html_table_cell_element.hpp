#pragma once
#ifndef SBROWSER_HTML_TABLE_CELL_ELEMENT_HPP
#define SBROWSER_HTML_TABLE_CELL_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_table_cell_element;}


class html::elements::html_table_cell_element : public html_element
{
public constructors:
    html_table_cell_element();

public js_properties:
    ext::html_property<ulong, _T> col_span;
    ext::html_property<ulong, _T> row_span;
    ext::html_property<ext::string, _T> headers;
    ext::html_property<long> cell_index;

    ext::html_property<ext::string, _T> scope;
    ext::html_property<ext::string, _T> abbr;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private accessors:
    auto get_cell_index() -> long;
};


#endif //SBROWSER_HTML_TABLE_CELL_ELEMENT_HPP
