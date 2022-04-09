#pragma once
#ifndef SBROWSER_TREES_HPP
#define SBROWSER_TREES_HPP

#include <ext/vector.hpp>

namespace dom::helpers {struct trees;}
namespace dom::nodes {class node;}
namespace dom::nodes {class text;}


struct dom::helpers::trees final
{
    // ancestors and descendants
    static auto root(
            const nodes::node* node)
            -> nodes::node*;

    static auto descendants(
            const nodes::node* node)
            -> ext::vector<nodes::node*>;

    static auto ancestors(
            const nodes::node* node)
            -> ext::vector<nodes::node*>;

    // tree checks
    static auto is_descendant(
            const nodes::node* node_a,
            const nodes::node* node_b)
            -> bool;

    static auto is_ancestor(
            const nodes::node* node_a,
            const nodes::node* node_b)
            -> bool;

    static auto is_sibling(
            const nodes::node* node_a,
            const nodes::node* node_b)
            -> bool;

    static auto is_preceding(
            const nodes::node* node_a,
            const nodes::node* node_b)
            -> bool;

    static auto is_following(
            const nodes::node* node_a,
            const nodes::node* node_b)
            -> bool;

    // indexing and length of nodes
    static auto index(
            const nodes::node* node_a)
            -> ulong;

    static auto length(
            const nodes::node* node_a)
            -> ulong;

    // lists of nodes preceding or following another node
    template <typename T=nodes::node*>
    static auto all_following(
            const nodes::node* node_a)
            -> ext::vector<nodes::node*>;

    template <typename T=nodes::node*>
    static auto all_preceding(
            const nodes::node* node_a)
            -> ext::vector<nodes::node*>;

    template <typename T=nodes::node*>
    static auto all_following_siblings(
            const nodes::node* node_a)
            -> ext::vector<nodes::node*>;

    template <typename T=nodes::node*>
    static auto all_preceding_siblings(
            const nodes::node* node_a)
            -> ext::vector<nodes::node*>;

    // node type checks (used for filters)
    static auto is_element_node(
            const nodes::node* node_a)
            -> bool;

    static auto is_text_node(
            const nodes::node* node_a)
            -> bool;

    static auto is_document_type_node(
            const nodes::node* node_a)
            -> bool;

    // text node helpers
    static auto descendant_text_content(
            const nodes::node* node_a)
            -> ext::string;

    static auto child_text_content(
            const nodes::node* node_a)
            -> ext::string;

    static auto descendant_text_nodes(
            const nodes::node* node_a)
            -> ext::vector<nodes::text*>;

    static auto contiguous_text_nodes(
            const nodes::node* node_a)
            -> ext::vector<nodes::text*>;

    static auto is_exclusive_text_node(
            const nodes::node* node_a)
            -> bool;

    // other general helper methods for nodes
    static auto common_ancestor(
            const nodes::node* node_a,
            const nodes::node* node_b)
            -> nodes::node*;

    static auto is_document_element(
            const nodes::node* node_a)
            -> bool;
};


#endif //SBROWSER_TREES_HPP
