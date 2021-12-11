#pragma once
#ifndef SBROWSER_TREES_HPP
#define SBROWSER_TREES_HPP

#include <ext/vector.hpp>

namespace dom {
    namespace helpers {struct trees;}
    namespace nodes {
        class node;
        class text;
    }
}


struct dom::helpers::trees {
    static nodes::node* root(nodes::node* node);
    static ext::vector<nodes::node*> descendants(nodes::node* node);
    static ext::vector<nodes::node*> ancestors(nodes::node* node);

    static bool is_descendant(nodes::node* node_a, nodes::node* node_b);
    static bool is_ancestor(nodes::node* node_a, nodes::node* node_b);
    static bool is_sibling(nodes::node* node_a, nodes::node* node_b);
    static bool is_preceding(nodes::node* node_a, nodes::node* node_b);
    static bool is_following(nodes::node* node_a, nodes::node* node_b);

    static unsigned long index(nodes::node* node_a);
    static unsigned long length(nodes::node* node_a);

    template <typename T=nodes::node*> static ext::vector<nodes::node*> all_following(nodes::node* node_a);
    template <typename T=nodes::node*> static ext::vector<nodes::node*> all_preceding(nodes::node* node_a);
    template <typename T=nodes::node*> static ext::vector<nodes::node*> all_following_siblings(nodes::node* node_a);
    template <typename T=nodes::node*> static ext::vector<nodes::node*> all_preceding_siblings(nodes::node* node_a);

    static bool is_element_node(nodes::node* node_a);
    static bool is_text_node(nodes::node* node_a);
    static bool is_document_type_node(nodes::node* node_a);

    static ext::string descendant_text_content(nodes::node* node_a);
    static ext::string child_text_content(nodes::node* node_a);
    static ext::vector<nodes::text*> descendant_text_nodes(nodes::node* node_a);
    static ext::vector<nodes::text*> contiguous_text_nodes(nodes::node* node_a);
    static bool is_excelusive_text_node(nodes::node* node_a);
};


#endif //SBROWSER_TREES_HPP
