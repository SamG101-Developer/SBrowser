#pragma once
#ifndef SBROWSER_TREES_HPP
#define SBROWSER_TREES_HPP

#include <ext/iterables/vector.hpp>

namespace dom
{
    namespace helpers {struct trees;}
    namespace nodes
    {
        class node;
        class text;
    }
}


struct dom::helpers::trees {
    static auto root(const nodes::node* node) -> nodes::node*;
    static auto descendants(const nodes::node* node) -> ext::vector<nodes::node*>;
    static auto ancestors(const nodes::node* node) -> ext::vector<nodes::node*>;

    static auto is_descendant(const nodes::node* node_a, const nodes::node* node_b) -> bool;
    static auto is_ancestor(const nodes::node* node_a, const nodes::node* node_b) -> bool;
    static auto is_sibling(const nodes::node* node_a, const nodes::node* node_b) -> bool;
    static auto is_preceding(const nodes::node* node_a, const nodes::node* node_b) -> bool;
    static auto is_following(const nodes::node* node_a, const nodes::node* node_b) -> bool;

    static auto index(const nodes::node* node_a) -> unsigned long;
    static auto length(const nodes::node* node_a) -> unsigned long;

    template <typename T=nodes::node*> static auto all_following(const nodes::node* node_a) -> ext::vector<nodes::node*>;
    template <typename T=nodes::node*> static auto all_preceding(const nodes::node* node_a) -> ext::vector<nodes::node*>;
    template <typename T=nodes::node*> static auto all_following_siblings(const nodes::node* node_a) -> ext::vector<nodes::node*>;
    template <typename T=nodes::node*> static auto all_preceding_siblings(const nodes::node* node_a) -> ext::vector<nodes::node*>;

    static auto is_element_node(const nodes::node* node_a) -> bool;
    static auto is_text_node(const nodes::node* node_a) -> bool;
    static auto is_document_type_node(const nodes::node* node_a) -> bool;

    static auto descendant_text_content(const nodes::node* node_a) -> ext::string;
    static auto child_text_content(const nodes::node* node_a) -> ext::string;
    static auto descendant_text_nodes(const nodes::node* node_a) -> ext::vector<nodes::text*>;
    static auto contiguous_text_nodes(const nodes::node* node_a) -> ext::vector<nodes::text*>;
    static auto is_exclusive_text_node(const nodes::node* node_a) -> bool;

    static auto common_ancestor(const nodes::node* node_a, const nodes::node* node_b) -> nodes::node*;
};


#endif //SBROWSER_TREES_HPP
