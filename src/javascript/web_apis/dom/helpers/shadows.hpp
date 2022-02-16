#pragma once
#ifndef SBROWSER_SHADOWS_HPP
#define SBROWSER_SHADOWS_HPP

#include <ext/iterables/vector.hpp>

namespace dom {
    namespace helpers {struct shadows;}
    namespace nodes {
        class event_target;
        class node;
        class shadow_root;
    }
}

namespace html::elements {class html_slot_element;}


struct dom::helpers::shadows { // TODO -> change to const nodes::node* etc where possible (fix compat issues w/ other helper structs)
    static bool is_connected(const nodes::node* node);
    static bool is_slot(const nodes::node* node);
    static bool is_slottable(const nodes::node* node);
    static bool is_assigned(const nodes::node* node);

    static html::elements::html_slot_element* find_slot(nodes::node* slottable, bool open_flag = false);
    static ext::vector<nodes::node*> find_slottables(html::elements::html_slot_element* slot);
    static ext::vector<nodes::node*> find_flattened_slottables(html::elements::html_slot_element* slot);

    static void assign_slot(nodes::node* slottable);
    static void assign_slottables(html::elements::html_slot_element* slot);
    static void assign_slottables_for_tree(const nodes::node* descendant);

    static void signal_slot_change(const nodes::node* slot);
    static nodes::node* shadow_including_root(const nodes::node* node_a);
    static nodes::event_target* retarget(const nodes::event_target* event_target_a, const nodes::event_target* event_target_b);
    static nodes::shadow_root* shadow_root(const nodes::node* node_a);

    static bool is_root_shadow_root(const nodes::node* node_a);
    static bool is_shadow_root(const nodes::node* node_a);
    static bool is_shadow_host(const nodes::node* node_a);
    static bool is_shadow_including_descendant(const nodes::node* node_a, const nodes::node* node_b);
    static bool is_shadow_including_ancestor(const nodes::node* node_a, const nodes::node* node_b);
    static bool is_host_including_ancestor(const nodes::node* node_a, const nodes::node* node_b);
    static bool is_closed_shadow_hidden(const nodes::node* node_a, const nodes::node* node_b);
};


#endif //SBROWSER_SHADOWS_HPP
