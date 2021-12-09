#pragma once
#ifndef SBROWSER_SHADOWS_HPP
#define SBROWSER_SHADOWS_HPP

#include <ext/vector.hpp>

namespace dom {
    namespace helpers {struct shadows;}
    namespace nodes {
        class event_target;
        class node;
    }
}

namespace html::elements {class html_slot_element;}


struct dom::helpers::shadows {
    static bool is_connected(nodes::node* node);
    static bool is_slot(nodes::node* node);
    static bool is_slottable(nodes::node* node);
    static bool is_assigned(nodes::node* node);

    static html::elements::html_slot_element* find_slot(nodes::node* slottable, bool open_flag);
    static ext::vector<nodes::node*> find_slottables(html::elements::html_slot_element* slot);
    static ext::vector<nodes::node*> find_flattened_slottables(html::elements::html_slot_element* slot);

    static void assign_slot(nodes::node* slottable);
    static void assign_slottables(html::elements::html_slot_element* slot);
    static void assign_slottables_for_tree(nodes::node* root);

    static void signal_slot_change(nodes::node* slot);
    static nodes::node* shadow_including_root(nodes::node* A);
    static nodes::event_target* retarget(nodes::event_target* A, nodes::event_target* B);

    static bool is_root_shadow_root(nodes::node* A);
    static bool is_shadow_root(nodes::node* A);
    static bool is_shadow_host(nodes::node* A);
    static bool is_shadow_including_descendant(nodes::node* A, nodes::node* B);
    static bool is_shadow_including_ancestor(nodes::node* A, nodes::node* B);
    static bool is_host_including_ancestor(nodes::node* A, nodes::node* B);
    static bool is_closed_shadow_hidden(nodes::node* A, nodes::node* B);
};


#endif //SBROWSER_SHADOWS_HPP
