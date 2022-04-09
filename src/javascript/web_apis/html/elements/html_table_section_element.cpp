#include "html_table_section_element.hpp"

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/namespaces.hpp>

#include <html/elements/html_table_row_element.hpp>


html::elements::html_table_section_element::html_table_section_element() = default;


auto html::elements::html_table_section_element::insert_row(
        ulong index)
        -> html::elements::html_table_row_element
{
    // if the index is outside -1 and the number of rows, throw an index error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "Index must be between -1 and the rows.length (inclusive)",
            [this, index] {return index <= -1 or index > rows->length();});

    // create the table row element in the document
    auto* table_row_element = dom::helpers::custom_elements::create_an_element<html_table_row_element>(owner_document, "tr", dom::helpers::namespaces::HTML);

    // either append or insert the row depending on the index
    index == -1 or index == rows->length()
            ? rows->append(table_row_element)
            : rows->insert(table_row_element, index);

    // return the newly created table row element
    return *table_row_element;
}


auto html::elements::html_table_section_element::delete_row(
        ulong index)
        -> void*
{
    // if the index is outside -1 and the number of rows, throw an index error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "Index must be between -1 and the rows.length (inclusive)",
            [this, index] {return index <= -1 or index > rows->length();});

    // pop the element from the list and return it
    rows->pop(index);
}


auto html::elements::html_table_section_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_table_section_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .function("insertRow", &html_table_section_element::insert_row)
            .function("deleteRow", &html_table_section_element::delete_row)
            .var("rows", &html_table_section_element::rows)
            .auto_wrap_objects();
}
