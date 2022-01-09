#pragma once
#ifndef SBROWSER_TREES_HPP
#define SBROWSER_TREES_HPP

#include <ext/iterables/vector.hpp>

namespace dom {
    namespace helpers {struct trees;}
    namespace nodes {
        class node;
        class text;
    }
}


struct dom::helpers::trees {
    static nodes::node* root(const nodes::node* node);
    static ext::vector<nodes::node*> descendants(const nodes::node* node);
    static ext::vector<nodes::node*> ancestors(const nodes::node* node);

    static bool is_descendant(const nodes::node* node_a, const nodes::node* node_b);
    static bool is_ancestor(const nodes::node* node_a, const nodes::node* node_b);
    static bool is_sibling(const nodes::node* node_a, const nodes::node* node_b);
    static bool is_preceding(const nodes::node* node_a, const nodes::node* node_b);
    static bool is_following(const nodes::node* node_a, const nodes::node* node_b);

    static unsigned long index(const nodes::node* node_a);
    static unsigned long length(const nodes::node* node_a);

    template <typename T=nodes::node*> static ext::vector<nodes::node*> all_following(const nodes::node* node_a);
    template <typename T=nodes::node*> static ext::vector<nodes::node*> all_preceding(const nodes::node* node_a);
    template <typename T=nodes::node*> static ext::vector<nodes::node*> all_following_siblings(const nodes::node* node_a);
    template <typename T=nodes::node*> static ext::vector<nodes::node*> all_preceding_siblings(const nodes::node* node_a);

    static bool is_element_node(const nodes::node* node_a);
    static bool is_text_node(const nodes::node* node_a);
    static bool is_document_type_node(const nodes::node* node_a);

    static ext::string descendant_text_content(const nodes::node* node_a);
    static ext::string child_text_content(const nodes::node* node_a);
    static ext::vector<nodes::text*> descendant_text_nodes(const nodes::node* node_a);
    static ext::vector<nodes::text*> contiguous_text_nodes(const nodes::node* node_a);
    static bool is_exclusive_text_node(const nodes::node* node_a);

    static nodes::node* common_ancestor(const nodes::node* node_a, const nodes::node* node_b);
};


#endif //SBROWSER_TREES_HPP
