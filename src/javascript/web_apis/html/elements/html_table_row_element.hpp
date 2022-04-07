#pragma once
#ifndef SBROWSER_HTML_TABLE_ROW_ELEMENT_HPP
#define SBROWSER_HTML_TABLE_ROW_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_table_row_element;}
namespace html::elements {class html_table_cell_element;}


class html::elements::html_table_row_element : public html_element
{
public constructors:
    html_table_row_element();

public js_methods:
    auto insert_cell(long index = -1) -> html_table_cell_element;
    auto delete_cell(long index = -1) -> void;

public js_properties:
    ext::html_property<long> row_index;
    ext::html_property<long> section_row_index;
    ext::html_property<ext::vector<html_table_cell_element*>*> cells;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private accessors:
    auto get_row_index() const -> long;
    auto get_section_row_index() const -> long;
    auto get_cells() const -> ext::vector<html_table_cell_element*>*
};


#endif //SBROWSER_HTML_TABLE_ROW_ELEMENT_HPP
