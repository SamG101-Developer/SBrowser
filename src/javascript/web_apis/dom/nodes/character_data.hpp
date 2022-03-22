#pragma once
#ifndef SBROWSER_CHARACTER_DATA_HPP
#define SBROWSER_CHARACTER_DATA_HPP

#include <dom/nodes/node.hpp>

#include <dom/mixins/child_node.hpp>
#include <dom/mixins/non_document_type_child_node.hpp>

namespace dom::nodes {class character_data;}


/*
 * https://dom.spec.whatwg.org/#interface-characterdata
 * https://developer.mozilla.org/en-US/docs/Web/API/CharacterData
 *
 * The CharacterData abstract interface represents a Node object that contains characters. This is an abstract
 * interface, meaning there aren't any objects of type CharacterData: it is implemented by other interfaces like Text,
 * Comment, CDATASection, or ProcessingInstruction, which aren't abstract.
 */
class dom::nodes::character_data
        : public node
        , public mixins::child_node<character_data>
        , public mixins::non_document_type_child_node<character_data>
{
public constructors:
    character_data();

public methods:
    auto substring_data(unsigned long offset, unsigned long count) const -> ext::string;
    auto append_data(const ext::string& new_data) -> void;
    auto insert_data(unsigned long offset, const ext::string& new_data) -> void;
    auto delete_data(unsigned long offset, unsigned long count) -> void;
    auto replace_data(unsigned long offset, unsigned long count, const ext::string& new_data) -> void;

public properties:
    ext::dom_property<ext::string, _F> data;
    ext::dom_property<unsigned long, _F> length;

public: internal_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private accessors:
    auto get_node_value() const -> ext::string override;
    auto get_text_content() const -> ext::string override;
    auto get_length() const -> unsigned long;

    auto set_node_value(const ext::string& val) -> void override;
    auto set_text_content(const ext::string& val) -> void override;
};


#endif //SBROWSER_CHARACTER_DATA_HPP
