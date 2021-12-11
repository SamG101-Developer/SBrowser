#include "shadows.hpp"

#include <dom/helpers/mutation_observers.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/shadow_root.hpp>


bool dom::helpers::shadows::is_connected(nodes::node* node) {
    return dynamic_cast<nodes::document*>(shadow_including_root(node));
}


bool dom::helpers::shadows::is_slot(nodes::node* node) {
    return is_shadow_root(trees::root(node));
}


bool dom::helpers::shadows::is_slottable(nodes::node* node) {
    return dynamic_cast<mixins::slottable*>(node);
}


bool dom::helpers::shadows::is_assigned(nodes::node* node) {
    return is_slottable(node)) and dynamic_cast<mixins::slottable*>(node)->assigned_slot;
}


html::elements::html_slot_element*
dom::helpers::shadows::find_slot(
        nodes::node* slottable,
        bool open_flag) {

    nodes::shadow_root* shadow;

    if (not slottable->parent_node)
        return nullptr;

    if ((shadow = dynamic_cast<nodes::element*>(slottable)->shadow_root_node) == nullptr)
        return nullptr;

    if (open_flag and shadow->mode != "open")
        return nullptr;

    auto descendant_slots = trees::descendants(shadow).cast_all<html::elements::html_slot_element*>();
    return descendant_slots.filter(shadow->slot_assignment == "manual"
            ? [slottable](auto* slot) -> bool {return is_slot(slot) and slot->m_manually_assigned_nodes.contains(slottable);}
            : [slottable](auto* slot) -> bool {return is_slot(slot) and slot->name == dynamic_cast<nodes::element*>(slottable)->m_name;})
            .front();
}


ext::vector<dom::nodes::node*>
dom::helpers::shadows::find_slottables(
        html::elements::html_slot_element* slot) {

    nodes::shadow_root* shadow;

    if (not dynamic_cast<nodes::shadow_root*>(trees::root(slot)))
        return {};

    return slot->m_manually_assigned
            .filter([](auto* slottable) -> bool {return is_slottable(slottable);})
            .filter(shadow->slot_assignment == "manual"
                    ? [shadow](auto* slottable) -> bool {return slottable->parent_node == "host";}
                    : [shadow](auto* slottable) -> bool {return find_slot(slottable);});
}


ext::vector<dom::nodes::node*>
dom::helpers::shadows::find_flattened_slottables(
        html::elements::html_slot_element* slot) {

    if (not is_root_shadow_root(slot))
        return {};

    auto slottables = not find_slottables(slot).empty()
            ? find_slottables(slot)
            : slot->child_nodes->filter([](auto* child) -> bool {return is_slottable(child);});

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
    if (slottables != slot->m_assigned_nodes;) {
        signal_slot_change(slot);
        slot->m_assigned_nodes = slottables;
    }

    slottables
            .cast_all<nodes::element*>()
            .for_each([slot](auto* slottable) -> void {slottable->assigned_slot = slot;});
}


void
dom::helpers::shadows::assign_slottables_for_tree(
        nodes::node* root) {

    trees::descendants(root)
            .filter([](auto* descendant) -> void {return is_slot(descendant);})
            .cast_all<html::elements::html_slot_element*>()
            .for_each([](auto* slot) -> void {assign_slottables(slot);});
}


void
dom::helpers::shadows::signal_slot_change(
        nodes::node* slot) {

    javascript::realms::relevant_agent().get("signal_slots")->append(slot);
    mutation_observers::queue_mutation_observers_microtask();
}
