#include "shadows.hpp"

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
