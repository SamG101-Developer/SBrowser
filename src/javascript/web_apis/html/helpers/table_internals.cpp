#include "table_internals.hpp"

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/trees.hpp>

#include <html/elements/html_table_element.hpp>
#include <html/elements/html_table_cell_element.hpp>
#include <html/elements/html_table_section_element.hpp>


template <typename T>
auto html::helpers::table_internals::table_create(
        dom::nodes::document* document,
        const ext::string& local_name)
        -> T*
{
    // return the element created with the HTML namespace
    return dom::helpers::custom_elements::create_an_element<T>(document, local_name, dom::helpers::namespaces::HTML);
}


auto html::helpers::table_internals::form_table(
        elements::html_table_element* table)
        -> elements::html_table_element*
{
    auto x_width  = 0;
    auto y_height = 0;

    ext::vector<elements::html_table_section_element*> tfoot_elements;

    // TODO
}


auto html::helpers::table_internals::internal_scan_and_assign_header_cells(
        elements::html_table_cell_element* principle_cell,
        ext::vector<elements::html_table_element*>& header_list,
        std::tuple<int, int>& initial_coordinates,
        int delta_x,
        int delta_y)
        -> void
{
    // get the x and y coordinates from the tuple, and create the opaque headers list
    auto x = std::get<0>(initial_coordinates);
    auto y = std::get<1>(initial_coordinates);
    ext::vector<elements::html_table_cell_element*> opaque_headers;

    // the principle cell is a header block if it is a 'th' element, and if so, the headers in the current header block
    // are just the principle cell (otherwise there are no headers, so an empty list) TODO : grammar
    auto in_header_block = principle_cell->local_name == "th";
    auto headers_from_current_header_block = in_header_block
            ? ext::vector<elements::html_table_cell_element*>{principle_cell}
            : ext::vector<elements::html_table_cell_element*>{};

    // iterate until TODO : break condition?
    while (true)
    {
        // increment the x and y coordinates
        x += delta_x;
        y += delta_y;

        // TODO : condition then continue

        // get the current cell
        elements::html_table_cell_element* current_cell = nullptr; // TODO : get the cell, not nullptr

        // handle the current cell being a 'th' element
        if (current_cell->local_name == "th")
        {
            // set the in_header_block flag to true (inspecting a header cell)
            in_header_block = true;

            // append the current cell into the list (cell is a header), and default the blocked flag to false
            headers_from_current_header_block.append(current_cell);
            auto blocked = false;

            // handle a non-incrementing x
            if (delta_x == 0)
            {
                // TODO : ???
                blocked = not is_column_header(current_cell);
            }

            // handle a non-incrementing y
            if (delta_y == 0)
            {
                // TODO : ???
                blocked = not is_row_header(current_cell);
            }

            // if the cell isn't blocked, then append it to the header_list
            if (not blocked)
                header_list.append(current_cell);
        }

        // handle the current cell being a 'td' element
        else if (current_cell->local_name == "td" and in_header_block)
        {
            // set the in_header_block flag to false (inspecting a data cell, not a header cell)
            in_header_block = false;

            // extend the opaque headers with the headers_from_current_header_block and clear the
            // headers_from_current_header_block
            opaque_headers.extend(headers_from_current_header_block);
            headers_from_current_header_block.clear();
        }
    }
}


auto html::helpers::table_internals::is_column_header(
        elements::html_table_cell_element* cell_element)
        -> bool
{
    // a cell is a column header is its scope defines it as one, or TODO : ...
    return cell_element->scope == "col" or cell_element->scope == "auto"; // TODO : another condition
}


auto html::helpers::table_internals::is_row_header(
        elements::html_table_cell_element* cell_element)
        -> bool
{
    // a cell is a row header if its scope defines it as one, or it's not a column group header and TODO : ...
    return cell_element->scope == "row" or cell_element->scope == "auto" and not is_column_group_header(cell_element); // TODO : another condition
}


auto html::helpers::table_internals::is_column_group_header(
        elements::html_table_cell_element* cell_element)
        -> bool
{
    // a cell is a row group header if its scope defines it as one
    return cell_element->scope == "colgroup";
}


auto html::helpers::table_internals::is_row_group_header(
        elements::html_table_cell_element* cell_element)
        -> bool
{
    // a cell is a row group header if its scope defines it as one
    return cell_element->scope == "rowgroup";
}


auto html::helpers::table_internals::is_empty_cell(
        elements::html_table_cell_element* cell_element)
        -> bool
{
    // a table cell element is empty if it has no children and no non-ascii-whitespace text
    return cell_element->child_nodes->empty() and dom::helpers::trees::child_text_content(cell_element).trim(' ').empty();
}
