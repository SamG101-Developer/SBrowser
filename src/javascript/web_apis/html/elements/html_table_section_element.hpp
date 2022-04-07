#pragma once
#ifndef SBROWSER_HTML_TABLE_SECTION_ELEMENT_HPP
#define SBROWSER_HTML_TABLE_SECTION_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_table_section_element;}
namespace html::elements {class html_table_row_element;}


class html::elements::html_table_section_element : public html_element
{
public constructors:
    html_table_section_element();

public js_methods:
    auto insert_row(unsigned long index = -1) -> html::elements::html_table_row_element;
    auto delete_row(unsigned long index = -1) -> void;

public js_properties:
    ext::html_property<ext::vector<html_table_row_element*>*> rows;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_TABLE_SECTION_ELEMENT_HPP
