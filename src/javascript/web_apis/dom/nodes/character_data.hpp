#pragma once
#ifndef SBROWSER_CHARACTER_DATA_HPP
#define SBROWSER_CHARACTER_DATA_HPP

#include <dom/nodes/node.hpp>
#include <dom/mixins/child_node.hpp>
#include <dom/mixins/non_document_type_child_node.hpp>

#include <QtWidgets/QLabel>

namespace dom::nodes {class character_data;}


class dom::nodes::character_data
        : public node
        , public mixins::child_node<character_data>
        , public mixins::non_document_type_child_node<character_data>
{
public constructors:
    character_data();

public js_methods:
    auto substring_data(unsigned long offset, unsigned long count) const -> ext::string;
    auto append_data(const ext::string& new_data) -> void;
    auto insert_data(unsigned long offset, const ext::string& new_data) -> void;
    auto delete_data(unsigned long offset, unsigned long count) -> void;
    auto replace_data(unsigned long offset, unsigned long count, const ext::string& new_data) -> void;

public js_properties:
    ext::dom_property<ext::string> data;
    ext::dom_property<unsigned long> length;

public cpp_methods:
    auto qt() const -> QLabel* override;
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private accessors:
    auto get_node_value() const -> ext::string override;
    auto get_text_content() const -> ext::string override;
    auto get_length() const -> unsigned long;

    auto set_node_value(const ext::string& val) -> void override;
    auto set_text_content(const ext::string& val) -> void override;
};


#endif //SBROWSER_CHARACTER_DATA_HPP
