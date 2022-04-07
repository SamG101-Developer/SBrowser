#pragma once
#ifndef SBROWSER_HTML_TABLE_ELEMENT_HPP
#define SBROWSER_HTML_TABLE_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_table_element;}
namespace html::elements {class html_table_caption_element;}
namespace html::elements {class html_table_row_element;}
namespace html::elements {class html_table_section_element;}


class html::elements::html_table_element : public html_element
{
public constructors:
    html_table_element();

public js_methods:
    auto create_caption() -> html_table_caption_element*;
    auto create_t_head() -> html_table_section_element*;
    auto create_t_foot() -> html_table_section_element*;
    auto create_t_body() -> html_table_section_element*;
    auto insert_t_row(long index = -1) -> html_table_row_element*;

    auto delete_caption() -> void;
    auto delete_t_head() -> void;
    auto delete_t_foot() -> void;
    auto delete_t_row(long index = -1) -> void;

public js_properties:
    ext::html_property<html_table_caption_element*, _T> caption;
    ext::html_property<html_table_section_element*, _T> t_head;
    ext::html_property<html_table_section_element*, _T> t_foot;
    ext::html_property<ext::vector<html::elements::html_table_section_element*>*, _T> t_bodies;
    ext::html_property<ext::vector<html::elements::html_table_row_element    *>*, _T> rows;

private accessors:
    auto get_caption() const -> html_table_caption_element*;
    auto get_t_head() const -> html_table_section_element*;
    auto get_t_foot() const -> html_table_section_element*;
    auto get_rows() const -> ext::vector<dom::nodes::element*>*;

    auto set_caption(html_table_caption_element* val) -> void;
    auto set_t_head(html_table_section_element* val) -> void;
    auto set_t_foot(html_table_section_element* val) -> void;
};


#endif //SBROWSER_HTML_TABLE_ELEMENT_HPP
