#include "shadows.hpp"

#include <javascript/environment/realms.hpp>

#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/mixins/slottable.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/shadow_root.hpp>

#include <html/elements/html_slot_element.hpp>


bool dom::helpers::shadows::is_connected(nodes::node* node) {
    return dynamic_cast<nodes::document*>(shadow_including_root(node));
}


bool dom::helpers::shadows::is_slot(nodes::node* node) {
    return is_shadow_root(trees::root(node));
}


bool dom::helpers::shadows::is_slottable(nodes::node* node) {
    return dynamic_cast<mixins::slottable<nodes::node>*>(node);
}


bool dom::helpers::shadows::is_assigned(nodes::node* node) {
    return is_slottable(node) and dynamic_cast<mixins::slottable<nodes::node>*>(node)->assigned_slot;
}


html::elements::html_slot_element*
dom::helpers::shadows::find_slot(
        nodes::node* slottable,
        bool open_flag) {

    nodes::shadow_root* shadow;

    if (not slottable->parent)
        return nullptr;

    if ((shadow = dynamic_cast<nodes::element*>(slottable)->shadow_root_node) == nullptr)
        return nullptr;

    if (open_flag and shadow->mode != "open")
        return nullptr;

    auto descendant_slots = trees::descendants(shadow).cast_all<html::elements::html_slot_element*>();
    return descendant_slots
            .filter(shadow->slot_assignment == "manual"
                    ? std::function{[slottable](html::elements::html_slot_element* slot) -> bool {return is_slot(slot) and slot->m_manually_assigned_nodes->contains(slottable);}}
                    : std::function{[slottable](html::elements::html_slot_element* slot) -> bool {return is_slot(slot) and slot->name == dynamic_cast<nodes::element*>(slottable)->m_name;}})
            .front();
}


ext::vector<dom::nodes::node*>
dom::helpers::shadows::find_slottables(
        html::elements::html_slot_element* slot) {

    auto* root = dynamic_cast<nodes::shadow_root*>(trees::root(slot));

    if (not root) return {};

    return slot->m_manually_assigned_nodes
            ->filter([](auto* slottable) {return is_slottable(slottable);})
            .filter(root->slot_assignment == "manual"
                    ? std::function{[host = root->host](html::elements::html_slot_element* slottable) -> bool {return slottable->parent == host;}}
                    : std::function{[host = root->host](html::elements::html_slot_element* slottable) -> bool {return find_slot(slottable);}})
            .remove(nullptr, true);
}


ext::vector<dom::nodes::node*>
dom::helpers::shadows::find_flattened_slottables(
        html::elements::html_slot_element* slot) {

    if (not is_root_shadow_root(slot))
        return {};

    auto slottables = not find_slottables(slot).empty()
            ? find_slottables(slot)
            : slot->child_nodes->filter([](auto* child) {return is_slottable(child);});

    return slottables.transform<nodes::node*>([](auto* slottable) -> nodes::node* {return is_slot(slottable) and is_root_shadow_root(slottable)
            ? find_flattened_slottables(slottable).front()
            : slottable;});
}


void
dom::helpers::shadows::assign_slot(
        nodes::node* slottable) {

    return assign_slottables(dynamic_cast<html::elements::html_slot_element*>(find_slot(slottable)));
}


void
dom::helpers::shadows::assign_slottables(
        html::elements::html_slot_element* slot) {

    auto slottables = find_slottables(slot);
    if (slottables != *slot->m_assigned_nodes) {
        signal_slot_change(slot);
        slot->m_assigned_nodes = &slottables;
    }

    slottables
            .cast_all<nodes::element*>()
            .for_each([slot](auto* slottable) {slottable->assigned_slot = slot;});
}


void
dom::helpers::shadows::assign_slottables_for_tree(
        nodes::node* root) {

    trees::descendants(root)
            .filter([](auto* descendant) {return is_slot(descendant);})
            .cast_all<html::elements::html_slot_element*>()
            .for_each([](auto* slot) {assign_slottables(slot);});
}


void
dom::helpers::shadows::signal_slot_change(
        nodes::node* slot) {

    using html::elements::html_slot_element;

    javascript::realms::relevant_agent().get<ext::vector<html_slot_element*>*>("signal_slots")->append(dynamic_cast<html_slot_element*>(slot));
    mutation_observers::queue_mutation_observers_microtask();
}


dom::nodes::node*
dom::helpers::shadows::shadow_including_root(
        nodes::node* node_a) {

    return is_root_shadow_root(node_a)
            ? shadow_including_root(dynamic_cast<nodes::shadow_root*>(trees::root(node_a))->host)
            : trees::root(node_a);
}


dom::nodes::event_target*
dom::helpers::shadows::retarget(
        nodes::event_target* event_target_a,
        nodes::event_target* event_target_b) {

    auto* node_a = dynamic_cast<nodes::node*>(event_target_a);
    auto* node_b = dynamic_cast<nodes::node*>(event_target_b);

    while (node_a and is_shadow_root(node_a) and not is_shadow_including_ancestor(trees::root(node_a), node_b))
        node_a = dynamic_cast<nodes::shadow_root*>(trees::root(node_a))->host;
    return node_a;
}


dom::nodes::shadow_root*
dom::helpers::shadows::shadow_root(nodes::node* node_a) {
    return dynamic_cast<nodes::shadow_root*>(trees::root(node_a));
}


bool
dom::helpers::shadows::is_root_shadow_root(nodes::node* node_a) {
    return is_shadow_root(trees::root(node_a));
}


bool
dom::helpers::shadows::is_shadow_root(nodes::node* node_a) {
    return dynamic_cast<nodes::shadow_root*>(node_a);
}


bool
dom::helpers::shadows::is_shadow_host(nodes::node* node_a) {
    return dynamic_cast<nodes::element*>(node_a)->shadow_root_node;
}


bool
dom::helpers::shadows::is_shadow_including_descendant(
        nodes::node* node_a,
        nodes::node* node_b) {

    return trees::is_descendant(node_a, node_b) or is_shadow_root(node_a) and is_shadow_including_descendant(dynamic_cast<nodes::shadow_root*>(node_a)->host, node_b);
}


bool
dom::helpers::shadows::is_shadow_including_ancestor(
        nodes::node* node_a,
        nodes::node* node_b) {

    return is_shadow_including_descendant(node_b, node_a);
}


bool
dom::helpers::shadows::is_host_including_ancestor(
        nodes::node* node_a,
        nodes::node* node_b) {

    return trees::is_descendant(node_a, node_b) or shadow_root(node_b) and shadow_root(node_b)->host and is_host_including_ancestor(node_a, shadow_root(node_b)->host);
}


bool
dom::helpers::shadows::is_closed_shadow_hidden(
        nodes::node* node_a,
        nodes::node* node_b) {

    auto* shadow_root_a = dynamic_cast<nodes::shadow_root*>(node_a);

    return is_root_shadow_root(node_a)
            and not is_shadow_including_ancestor(shadow_root_a, node_b)
            and (shadow_root_a->mode == "closed" or is_closed_shadow_hidden(shadow_root_a->host, node_b));
}
