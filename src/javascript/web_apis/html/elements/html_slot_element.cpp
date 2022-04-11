#include "html_slot_element.hpp"

#include <dom/helpers/shadows.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/mixins/slottable.hpp>

#include <dom/nodes/element.hpp>
#include <dom/nodes/text.hpp>


auto html::elements::html_slot_element::assigned_nodes(
        const ext::string_any_map& options)
        -> ext::vector<dom::nodes::node*>*
{
    // return the assigned nodes if the options haven't set the flatten option, otherwise find the flattened slottables
    return not options.at("flatten").to<bool>()
            ? m_assigned_nodes
            : dom::helpers::shadows::find_flattened_slottables(this);
}


auto html::elements::html_slot_element::assigned_elements(
        const ext::string_any_map& options)
        -> ext::vector<dom::nodes::element*>*
{
    // return the assigned nodes that are elements
    return assigned_nodes(options)->cast_all<dom::nodes::element*>()
}


template <typename ...Args>
auto html::elements::html_slot_element::assign(
        Args... nodes)
        -> void // TODO : improve this method using the dom::mixins::slottable<T> mixin somehow (templated is the issue)
{
    // for each manually assigned node, set the slot assignment to nullptr
    m_manually_assigned_nodes->template for_each([]<typename T>(dom::mixins::slottable<T>* node) {node->m_manual_slot_assignment = nullptr;});

    // create a new nodes set
    auto nodes_set = new ext::vector<dom::nodes::node*>{};

    // iterate the element nodes
    for (auto* node: m_manually_assigned_nodes->template cast_all<dom::nodes::element*>())
    {
        if (node->m_manual_slot_assignment) node->m_manual_slot_assignment->m_manually_assigned_nodes->remove(node);
        node->m_manual_slot_assignment = this;
        nodes_set->append(node);
    }

    // iterate the text nodes
    for (auto* node: m_manually_assigned_nodes->template cast_all<dom::nodes::text*>())
    {
        if (node->m_manual_slot_assignment) node->m_manual_slot_assignment->m_manually_assigned_nodes->remove(node);
        node->m_manual_slot_assignment = this;
        nodes_set->append(node);
    }

    // set the manually assigned nodes to the node set, and assign the slottables for the tree of the root of this node
    m_manually_assigned_nodes = nodes_set;
    dom::helpers::shadows::assign_slottables_for_tree(dom::helpers::trees::root(this));
}


auto html::elements::html_slot_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_slot_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .function("assignedNodes", &html_slot_element::assigned_nodes)
            .function("assignedElements", &html_slot_element::assigned_elements)
            .function("assign", &html_slot_element::assign<>)
            .auto_wrap_objects();
}
