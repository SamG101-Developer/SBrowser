#pragma once
#ifndef SBROWSER_TABLE_INTERNALS_HPP
#define SBROWSER_TABLE_INTERNALS_HPP

#include <ext/string.hpp>

namespace dom::nodes {class document;}
namespace html::elements {class html_element;}
namespace html::elements {class html_table_cell_element;}
namespace html::elements {class html_table_row_element;}
namespace html::elements {class html_table_element;}
namespace html::helpers {struct table_internals;}


struct html::helpers::table_internals
{
    template <typename T>
    static auto table_create(
            dom::nodes::document* document,
            const ext::string& local_name)
            -> T*;

    static auto form_table(
            elements::html_table_element* table)
            -> elements::html_table_element*;

    static auto process_row_groups(
            elements::html_element* element)
            -> void;

    static auto end_row_group()
            -> void;

    static auto process_rows(
            elements::html_table_row_element* row_element)
            -> void;

    static auto assign_header_cells(
            elements::html_table_cell_element* principle_cell)
            -> void;

    static auto internal_scan_and_assign_header_cells(
            elements::html_table_cell_element* principle_cell,
            ext::vector<elements::html_table_element*>& header_list,
            std::tuple<int, int>& initial_coordinates,
            int delta_x,
            int delta_y)
            -> void;

    static auto is_column_header(
            elements::html_table_cell_element* cell_element)
            -> bool;

    static auto is_row_header(
            elements::html_table_cell_element* cell_element)
            -> bool;

    static auto is_column_group_header(
            elements::html_table_cell_element* cell_element)
            -> bool;

    static auto is_row_group_header(
            elements::html_table_cell_element* cell_element)
            -> bool;

    static auto is_empty_cell(
            elements::html_table_cell_element* cell_element)
            -> bool;
};


#endif //SBROWSER_TABLE_INTERNALS_HPP
