#include "shadows.hpp"

#include <javascript/environment/realms.hpp>

#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/mixins/slottable.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/shadow_root.hpp>

#include <html/elements/html_slot_element.hpp>


auto dom::helpers::shadows::is_connected(const nodes::node* node) -> bool
{
    // a node is connected if the shadow including root is a document
    return dynamic_cast<nodes::document*>(shadow_including_root(node));
}


auto dom::helpers::shadows::is_slot(const nodes::node* node) -> bool
{
    // a node is a slot if the root of the node is a shadow node
    return is_root_shadow_root(node);
}


auto dom::helpers::shadows::is_slottable(const nodes::node* node) -> bool
{
    // a node is a slottable if it inherits the slottable mixin
    return dynamic_cast<const mixins::slottable<nodes::node>*>(node);
}


auto dom::helpers::shadows::is_assigned(const nodes::node* node) -> bool
{
    // a node is assigned if it is a slottable, and it has an assigned slot
    return is_slottable(node) and dynamic_cast<const mixins::slottable<nodes::node>*>(node)->assigned_slot;
}


auto dom::helpers::shadows::find_slot(
        const nodes::node* slottable,
        bool open_flag)
        -> html::elements::html_slot_element*
{
    // create a pointer for the shadow root
    nodes::shadow_root* shadow;

    // return nullptr if the slottable doesn't have a parent
    if (not slottable->parent)
        return nullptr;

    // return nullptr if the slottable (as an element) doesn't have a shadow root
    if (not (shadow = dynamic_cast<const nodes::element*>(slottable)->shadow_root_node))
        return nullptr;

    // return nullptr if the open flag is set, but the shadow's mode is not open
    if (open_flag and shadow->mode != "open")
        return nullptr;

    // return the descendants of the shadow, (cast as html slot elements), whose TODO : cba to comment rn
    auto descendant_slots = trees::descendants(shadow).cast_all<html::elements::html_slot_element*>();
    return descendant_slots
            .filter(shadow->slot_assignment == "manual"
                    ? std::function{[slottable](const html::elements::html_slot_element* slot) -> bool {return is_slot(slot) and slot->m_manually_assigned_nodes->contains(slottable);}}
                    : std::function{[slottable](const html::elements::html_slot_element* slot) -> bool {return is_slot(slot) and slot->name == dynamic_cast<const nodes::element*>(slottable)->m_name;}})
            .front();
}


auto dom::helpers::shadows::find_slottables(const html::elements::html_slot_element* slot) -> ext::vector<dom::nodes::node*>
{
    // get the shadow root node, returning nullptr if the root is not a shadow root
    nodes::shadow_root* root = dynamic_cast<nodes::shadow_root*>(trees::root(slot));
    if (not root)
        return nullptr;

    // TODO : cba to comment rn
    return slot->m_manually_assigned_nodes->filter([](nodes::node* slottable) {return is_slottable(slottable);})
            .filter(root->slot_assignment == "manual"
                    ? std::function{[host = root->host](nodes::node* slottable) -> bool {return slottable->parent == host;}}
                    : std::function{[host = root->host](nodes::node* slottable) -> bool {return find_slot(slottable);}});
}


auto dom::helpers::shadows::find_flattened_slottables(const html::elements::html_slot_element* slot) -> ext::vector<dom::nodes::node*>
{
    // return nullptr if the slot's root is not a shadow root
    if (not is_root_shadow_root(slot))
        return nullptr;

    // the slottables are the result of find_slottables if it has 1 or more items in, otherwise the slottable children
    // of the slot
    auto slottables = not find_slottables(slot).empty()
            ? find_slottables(slot)
            : slot->child_nodes->filter([](nodes::node* child) {return is_slottable(child);});

    // TODO : cba
    return slottables.transform<>([](nodes::node* slottable) -> nodes::node* {return is_slot(slottable) and is_root_shadow_root(slottable) ? find_flattened_slottables(slottable).front() : slottable;});
}


auto dom::helpers::shadows::assign_slot(nodes::node* slottable) -> void
{
    // assign a slot by assigning slottables to the found slot for the slottable
    return assign_slottables(find_slot(slottable));
}


auto dom::helpers::shadows::assign_slottables(html::elements::html_slot_element* slot) -> void
{
    // the slottables are the found slottables for the slot
    auto slottables = find_slottables(slot);

    // if there found slottables aren't the same as the slot's assigned nodes
    if (slottables != *slot->m_assigned_nodes)
    {
        //signal the slot change, and set the slot's assigned nodes to the new slottables
        signal_slot_change(slot);
        slot->m_assigned_nodes = &slottables;
    }

    // for each element in the slottable list, set their assigned_slot to the slot
    slottables
            .cast_all<nodes::element*>()
            .for_each([slot](auto* slottable) {slottable->assigned_slot = slot;});
}


auto dom::helpers::shadows::assign_slottables_for_tree(const nodes::node* root) -> void
{
    // assign slottables for each html slot element in the descendants of the root TODO : filter needed?
    trees::descendants(root)
            .filter([](auto* descendant) {return is_slot(descendant);})
            .cast_all<html::elements::html_slot_element*>()
            .for_each([](auto* slot) {assign_slottables(slot);});
}


auto dom::helpers::shadows::signal_slot_change(const nodes::node* slot) -> void
{
    // append a nwe slot to the signal_slots list in the javascript context, and observe the microtask
    javascript::realms::relevant_agent().get<ext::vector<const html::elements::html_slot_element*>*>("signal_slots")->append(dynamic_cast<const html::elements::html_slot_element*>(slot));
    mutation_observers::queue_mutation_observers_microtask();
}


dom::nodes::node*
dom::helpers::shadows::shadow_including_root(const nodes::node* node_a)
{
    // return the shadow including host if the shadow root's host's root if the node's root is a shadow root, otherwise
    // the root of the node (not a shadow root)
    return is_root_shadow_root(node_a)
            ? shadow_including_root(dynamic_cast<nodes::shadow_root*>(trees::root(node_a))->host)
            : trees::root(node_a);
}


auto dom::helpers::shadows::retarget(
        const nodes::event_target* event_target_a,
        const nodes::event_target* event_target_b)
        -> dom::nodes::event_target*
{
    // convert the event targets to nodes
    auto* node_a = dynamic_cast<const nodes::node*>(event_target_a);
    auto* node_b = dynamic_cast<const nodes::node*>(event_target_b);

    // TODO : cba
    while (node_a and is_shadow_root(node_a) and not is_shadow_including_ancestor(trees::root(node_a), node_b))
        node_a = dynamic_cast<nodes::shadow_root*>(trees::root(node_a))->host;

    // return node_a TODO : casting correct here?
    return const_cast<nodes::node*>(node_a);
}


auto dom::helpers::shadows::shadow_root(const nodes::node* node_a) -> dom::nodes::shadow_root*
{
    // return the shadow_root cast root of the node
    return dynamic_cast<nodes::shadow_root*>(trees::root(node_a));
}


auto dom::helpers::shadows::is_root_shadow_root(const nodes::node* node_a) -> bool
{
    // return if the root of the tree is a shadow root
    return is_shadow_root(trees::root(node_a));
}


auto dom::helpers::shadows::is_shadow_root(const nodes::node* node_a) -> bool
{
    // return if the node cast to a shadow root is nullptr or not
    return dynamic_cast<const nodes::shadow_root*>(node_a);
}


auto dom::helpers::shadows::is_shadow_host(const nodes::node* node_a) -> bool
{
    // return if the node cast to an element has a shadow root node (ie is hosted)
    return dynamic_cast<const nodes::element*>(node_a)->shadow_root_node;
}


auto dom::helpers::shadows::is_shadow_including_descendant(
        const nodes::node* node_a,
        const nodes::node* node_b)
        -> bool
{
    // return if node_b is a descendant of node_a (normal text), or node_a is a shadow root and node_a's host is a
    // shadow root that is a shadow including descendant of node_b
    return trees::is_descendant(node_a, node_b) or is_shadow_root(node_a) and is_shadow_including_descendant(dynamic_cast<const nodes::shadow_root*>(node_a)->host, node_b);
}


auto dom::helpers::shadows::is_shadow_including_ancestor(
        const nodes::node* node_a,
        const nodes::node* node_b)
        -> bool
{
    // return if node_b is a shadow including descendant of node_a -> this means that node_a is a shadow including
    // ancestor of node_b
    return is_shadow_including_descendant(node_b, node_a);
}


auto dom::helpers::shadows::is_host_including_ancestor(
        const nodes::node* node_a,
        const nodes::node* node_b)
        -> bool
{
    // TODO
    return trees::is_descendant(node_a, node_b) or shadow_root(node_b) and shadow_root(node_b)->host and is_host_including_ancestor(node_a, shadow_root(node_b)->host);
}


auto dom::helpers::shadows::is_closed_shadow_hidden(
        const nodes::node* node_a,
        const nodes::node* node_b)
        -> bool
{
    // get the shadow root of node_a
    auto* shadow_root_a = dynamic_cast<const nodes::shadow_root*>(node_a);

    // return if the root is a shadow root that isn't a shadow including ancestor of node_b, and [the shadow root's mode
    // is closed or the shadow root's host is a closed shadow hidden of node_b]
    return is_root_shadow_root(node_a)
            and not is_shadow_including_ancestor(shadow_root_a, node_b)
            and (shadow_root_a->mode == "closed" or is_closed_shadow_hidden(shadow_root_a->host, node_b));
}
