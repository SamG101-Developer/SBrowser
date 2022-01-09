#pragma once
#ifndef SBROWSER_CHARACTER_DATA_HPP
#define SBROWSER_CHARACTER_DATA_HPP

#include <dom/nodes/node.hpp>

#include <dom/mixins/child_node.hpp>
#include <dom/mixins/non_document_type_child_node.hpp>

namespace dom::nodes {class character_data;}


class dom::nodes::character_data
        : public node
        , public mixins::child_node<character_data>
        , public mixins::non_document_type_child_node<character_data> {

public constructors:
    character_data();

public methods:
    ext::string substring_data(unsigned long offset, unsigned long count) const;
    void append_data(ext::cstring& new_data);
    void insert_data(unsigned long offset, ext::cstring& new_data);
    void delete_data(unsigned long offset, unsigned long count);
    void replace_data(unsigned long offset, unsigned long count, ext::cstring& new_data);

public properties:
    ext::dom_property<ext::string> data;
    ext::dom_property<unsigned long> length;

private accessors:
    inline ext::string get_node_value() const override;
    inline ext::string get_text_content() const override;
    inline unsigned long get_length() const;

    inline void set_node_value(ext::cstring& val) override;
    inline void set_text_content(ext::cstring& val) override;
};


#endif //SBROWSER_CHARACTER_DATA_HPP
