#pragma once
#ifndef SBROWSER_SHADOWS_HPP
#define SBROWSER_SHADOWS_HPP

#include <ext/vector.hpp>

namespace dom {
    namespace helpers {struct shadows;}
    namespace nodes {class node;}
}

namespace html::elements {class html_slot_element;}


struct dom::helpers::shadows {
    static bool is_connected(nodes::node* node);
    static bool is_slot(nodes::node* node);
    static bool is_slottable(nodes::node* node);
    static bool is_assigned(nodes::node* node);

    static html::elements::html_slot_element find_slot(nodes::node* slottable, bool open_flag);
    static ext::vector<nodes::node*> find_slottables(html::elements::html_slot_element* slot);
    static ext::vector<nodes::node*> find_flattened_slottables(html::elements::html_slot_element* slot);

    static void assign_slot(nodes::node* slottable);
    static void assign_slottables(html::elements::html_slot_element* slot);
    static void assign_slottables_for_tree(nodes::node* root);

    static void signal_slot_change(nodes::node* slot);
};


#endif //SBROWSER_SHADOWS_HPP
