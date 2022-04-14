#include "html_table_row_element.hpp"

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/trees.hpp>

#include <html/elements/html_table_element.hpp>
#include <html/elements/html_table_cell_element.hpp>
#include <html/elements/html_table_section_element.hpp>


html::elements::html_table_row_element::html_table_row_element()
        : html_element()
{
    // set the property values
    cells = new ext::vector<html_table_cell_element*>{};
}


auto html::elements::html_table_row_element::insert_cell(
        long index)
        -> html_table_cell_element
{
    // if the index is outside -1 and the number of rows, throw an index error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "Index must be between -1 and the rows.length (inclusive)",
            [this, index] {return index <= -1 or index > cells->length();});

    // create the table row element in the document
    auto* table_cell_element = dom::helpers::custom_elements::create_an_element<html_table_cell_element>(owner_document, "tr", dom::helpers::namespaces::HTML);

    // either append or insert the row depending on the index
    index == -1 or index == cells->length()
    ? cells->append(table_cell_element)
    : cells->insert(table_cell_element, index);

    // return the newly created table row element
    return *table_cell_element;
}


auto html::elements::html_table_row_element::delete_cell(
        long index)
        -> void
{
    // if the index is outside -1 and the number of rows, throw an index error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "Index must be between -1 and the rows.length (inclusive)",
            [this, index] {return index <= -1 or index > cells->length();});

    // pop the element from the list and return it
    cells->pop(index);
}


auto html::elements::html_table_row_element::get_row_index() const
        -> long
{
    // the table element is either the parent element, or the grandparent element if the parent element is a table
    // section
    auto* table_element = ext::property_dynamic_cast<html_table_element*>(parent_element);
    if (not table_element and ext::property_dynamic_cast<html_table_section_element*>(parent_element))
        table_element = ext::property_dynamic_cast<html_table_element*>(parent_element->parent_element);

    // return the index of the row in the table's rows list if there is a table element
    if (table_element)
        return table_element->rows->find(this);

    // otherwise, return -1
    return -1;
}


auto html::elements::html_table_row_element::get_section_row_index() const
        -> long
{
    if (auto* table_parent = ext::property_dynamic_cast<html_table_element*>(parent_element))
        return table_parent->rows->find(this);

    if (auto* table_section_parent = ext::property_dynamic_cast<html_table_section_element*>(parent_element))
        return table_section_parent->rows->find(this);

    return -1;
}


auto html::elements::html_table_row_element::get_cells() const
        -> ext::vector<html_table_cell_element*>*
{
    return children->cast_all<html_table_cell_element*>().filter([](html_table_cell_element* element) {return element->local_name == "td" or element->local_name == "th";});
}


auto html::elements::html_table_row_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_table_row_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .function("insertCell", &html_table_row_element::insert_cell)
            .function("deleteCell", &html_table_row_element::delete_cell)
            .var("rowIndex", &html_table_row_element::row_index)
            .var("sectionRowIndex", &html_table_row_element::section_row_index)
            .var("cells", &html_table_row_element::cells)
            .auto_wrap_objects();
}
