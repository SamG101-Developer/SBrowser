#pragma once
#ifndef SBROWSER_SHADOWS_HPP
#define SBROWSER_SHADOWS_HPP

#include <ext/vector.hpp>


namespace dom::helpers {struct shadows;}
namespace dom::nodes {class event_target;}
namespace dom::nodes {class node;}
namespace dom::nodes {class shadow_root;}
namespace html::elements {class html_slot_element;}


struct dom::helpers::shadows final
{
    // shadow checks
    static auto is_connected(
            const nodes::node* node)
            -> bool;

    static auto is_slot(
            const nodes::node* node)
            -> bool;

    static auto is_slottable(
            const nodes::node* node)
            -> bool;

    static auto is_assigned(
            const nodes::node* node)
            -> bool;

    // tree based shadow checks
    static auto is_root_shadow_root(
            const nodes::node* node_a)
            -> bool;

    static auto is_shadow_root(
            const nodes::node* node_a)
            -> bool;

    static auto is_shadow_host(
            const nodes::node* node_a)
            -> bool;

    static auto shadow_including_descendants(
            const nodes::node* node_a)
            -> ext::vector<nodes::node*>;

    static auto is_shadow_including_descendant(
            const nodes::node* node_a,
            const nodes::node* node_b)
            -> bool;

    static auto is_shadow_including_ancestor(
            const nodes::node* node_a,
            const nodes::node* node_b)
            -> bool;

    static auto is_host_including_ancestor(
            const nodes::node* node_a,
            const nodes::node* node_b)
            -> bool;

    static auto is_closed_shadow_hidden(
            const nodes::node* node_a,
            const nodes::node* node_b)
            -> bool;

    // find slots and slottables
    static auto find_slot(
            const nodes::node* slottable,
            bool open_flag = false)
            -> html::elements::html_slot_element*;

    static auto find_slottables(
            const html::elements::html_slot_element* slot)
            -> ext::vector<nodes::node*>;

    static auto find_flattened_slottables(
            const html::elements::html_slot_element* slot)
            -> ext::vector<nodes::node*>;

    // assign slots and slottables
    static auto assign_slot(
            const nodes::node* slottable)
            -> void;

    static auto assign_slottables(
            const html::elements::html_slot_element* slot)
            -> void;

    static auto assign_slottables_for_tree(
            const nodes::node* descendant)
            -> void;

    // other general helper methods for shadows
    static auto signal_slot_change(
            const nodes::node* slot)
            -> void;

    static auto shadow_including_root(
            const nodes::node* node_a)
            -> nodes::node*;

    static auto retarget(
            const nodes::event_target* event_target_a,
            const nodes::event_target* event_target_b)
            -> nodes::event_target*;

    static auto shadow_root(
            const nodes::node* node_a)
            -> nodes::shadow_root*;
};


#endif //SBROWSER_SHADOWS_HPP
