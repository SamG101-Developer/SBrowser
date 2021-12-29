#include "mutation_algorithms.hpp"

#include <dom/nodes/comment.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/processing_instruction.hpp>
#include <dom/nodes/node.hpp>
#include <dom/nodes/shadow_root.hpp>
#include <dom/nodes/text.hpp>

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/exceptions.hpp>


void
dom::helpers::mutation_algorithms::common_checks(
        nodes::node* node,
        nodes::node* parent,
        nodes::node* child) {

    exceptions::throw_v8_exception(
            "parent must be a document, document_fragment or element node",
            HIERARCHY_REQUEST_ERR,
            [node] {return dynamic_cast<nodes::document*>(node) or dynamic_cast<nodes::document_fragment*>(node) or dynamic_cast<nodes::element*>(node);});

    exceptions::throw_v8_exception(
            "parent can not be a host-including ancestor of node",
            HIERARCHY_REQUEST_ERR,
            [node, parent] {return shadows::is_host_including_ancestor(node, parent);});

    exceptions::throw_v8_exception(
            "child's current parent does not equal parent",
            NOT_FOUND_ERR,
            [parent, child] {return child and child->parent != parent;});

    exceptions::throw_v8_exception(
            "node must be a document_fragment, document_type, element, text, processing_instruction or comment node",
            HIERARCHY_REQUEST_ERR,
            [node] {return not (dynamic_cast<nodes::document_fragment*>(node) or dynamic_cast<nodes::document_type*>(node) or dynamic_cast<nodes::element*>(node) or dynamic_cast<nodes::text*>(node) or dynamic_cast<nodes::processing_instruction*>(node) or dynamic_cast<nodes::comment*>(node));});

    exceptions::throw_v8_exception(
            "document parents can not have a text node child",
            HIERARCHY_REQUEST_ERR,
            [node, parent] {return dynamic_cast<nodes::document*>(parent) and dynamic_cast<nodes::text*>(node);});

    exceptions::throw_v8_exception(
            "document_type nodes can not have a non-document parent",
            HIERARCHY_REQUEST_ERR,
            [node, parent] {return dynamic_cast<nodes::document_type*>(node) and not dynamic_cast<nodes::document*>(parent);});
}


void
dom::helpers::mutation_algorithms::ensure_pre_insertion_validity(
        nodes::node* node,
        nodes::node* parent,
        nodes::node* child) {

    common_checks(node, parent, child);

    if (dynamic_cast<nodes::document*>(parent)) {
        if (dynamic_cast<nodes::document_fragment*>(node)) {

            exceptions::throw_v8_exception(
                    "document_fragment node with a document parent cannot have > 1 element children",
                    HIERARCHY_REQUEST_ERR,
                    [node] {return node->child_nodes->filter(&trees::is_element_node).length() > 1;});

            exceptions::throw_v8_exception(
                    "document_fragment node with a document parent cannot have > 0 text node children",
                    HIERARCHY_REQUEST_ERR,
                    [node] {return node->child_nodes->filter(&trees::is_text_node).length() > 0;});

            if (node->child_nodes->filter(&trees::is_element_node).length() == 1) {

                exceptions::throw_v8_exception(
                        "document_fragment node with a document parent and element child can not have an element sibling",
                        HIERARCHY_REQUEST_ERR,
                        [parent] {return parent->child_nodes->filter(&trees::is_element_node).length() > 0;});

                exceptions::throw_v8_exception(
                        "document_fragment node with a document parent and element child can not be inserted before a document_type node",
                        HIERARCHY_REQUEST_ERR,
                        [child] {return dynamic_cast<nodes::document_type*>(child);});

                exceptions::throw_v8_exception(
                        "document_fragment node with a document parent and element child can not be inserted before a document_type node",
                        HIERARCHY_REQUEST_ERR,
                        [child] {return child and not trees::all_following<nodes::document_type*>(child).empty();});
            }
        }

        else if (dynamic_cast<nodes::element*>(node)) {

            exceptions::throw_v8_exception(
                    "element node with a document parent can not have > 0 element children",
                    HIERARCHY_REQUEST_ERR,
                    [node] {return node->child_nodes->filter(&trees::is_element_node).length() > 0;});

            exceptions::throw_v8_exception(
                    "element node with a document parent can not be inserted before a document type node",
                    HIERARCHY_REQUEST_ERR,
                    [child] {return dynamic_cast<nodes::document_type*>(child);});

            exceptions::throw_v8_exception(
                    "element node with a document parent can not be inserted before a document type node",
                    HIERARCHY_REQUEST_ERR,
                    [child] {return child and not trees::all_following<nodes::document_type*>(child).empty();});
        }

        else if (dynamic_cast<nodes::document_type*>(node)) {

            exceptions::throw_v8_exception(
                    "document_type node with a document parent can not have document_type siblings",
                    HIERARCHY_REQUEST_ERR,
                    [parent] {return parent->child_nodes->filter(&trees::is_document_type_node);});

            exceptions::throw_v8_exception(
                    "document_type_node with a document parent can not be inserted after any elements",
                    HIERARCHY_REQUEST_ERR,
                    [child] {return child and not trees::all_preceding<nodes::element*>(child).emprt();});

            exceptions::throw_v8_exception(
                    "document_type node with a document parent can not have > 0 element children",
                    HIERARCHY_REQUEST_ERR,
                    [child, parent] {not child and parent->child_nodes->filter(&trees::is_element_node).length() > 0;});
        }
    }
}


dom::nodes::node*
dom::helpers::mutation_algorithms::pre_insert(
        nodes::node* node,
        nodes::node* parent,
        nodes::node* child) {

    ensure_pre_insertion_validity(node, parent, child);
    auto* reference_child = child == node ? node->next_sibling : child;
    insert(node, parent, reference_child);

    return node;
}


dom::nodes::node*
dom::helpers::mutation_algorithms::pre_remove(
        nodes::node* node,
        nodes::node* parent) {

    exceptions::throw_v8_exception(
            "node's current parent does not equal parent",
            NOT_FOUND_ERR,
            [node, parent] {return node->parent != parent;});

    return remove(node);
}


dom::nodes::node*
dom::helpers::mutation_algorithms::insert(
        nodes::node* node,
        nodes::node* parent,
        nodes::node* child,
        bool suppress_observers_flag) {

    // collect descendant nodes to insert as-well (as children of the node being inserted), empty doc_frag not inserted
    auto added_nodes = dynamic_cast<nodes::document_fragment*>(node) ? *node->child_nodes : ext::vector<nodes::node*>{node};
    auto count = added_nodes.length();
    if (count <= 0)
        return nullptr;

    // queue tree mutations for document fragment nodes being inserted
    if (dynamic_cast<nodes::document_fragment*>(node)) {
        node->child_nodes->for_each([](auto* child_node) {remove(child_node, true);});
        mutation_observers::queue_tree_mutation_record(node, {}, added_nodes, nullptr, nullptr);
    }

    // update ranges where one of the contains is the parent and the offset is after the anchor child
    if (child) {
        ext::vector<ranges::range*> live_ranges = javascript::realms::current_environment().get("live_ranges");
        live_ranges
                .filter([child, parent](auto* range) {return range->start_container == parent and range->start_offset > trees::index(child);})
                .for_each([count](auto* range) {range->start_offset += count;});

        live_ranges
                .filter([child, parent](auto* range) {return range->end_container == parent and range->end_offset > trees::index(child);})
                .for_each([count](auto* range) {range->end_offset += count;});
    }

    // determine the previous sibling (mutation records) and insert the node
    nodes::node* previous_sibling = child ? child->previous_sibling : parent->last_child;
    for (nodes::node* node_to_add: added_nodes) {

        // adopt the node into the document, and append / insert it into the child_nodes list
        parent->owner_document->adopt_node(node_to_add);
        not child ? parent->child_nodes->append(node_to_add) : parent->child_nodes->insert(node_to_add, parent->child_nodes->find(child));

        if (shadows::is_shadow_host(parent)
                and shadows::is_slottable(child_node)
                and dynamic_cast<nodes::shadow_root*>(trees::root(parent))->slot_assignment == "named")
            shadows::assign_slot(child_node);

        if (shadows::is_root_shadow_root(parent)
                and shadows::is_slot(parent)
                and dynamic_cast<html::elements::html_slot_element*>(parnet)->assigned_nodes->length() <= 0)
            shadows::signal_slot_change(parent);

        shadows::assign_slottables_for_tree()
    }

    if (not suppress_observers_flag)
        mutation_observers::queue_tree_mutation_record(parent, added_nodes, {}, previous_sibling, child);
    return node;
}


dom::nodes::node*
dom::helpers::mutation_algorithms::append(
        nodes::node* node,
        nodes::node* parent) {

    return pre_insert(node, parent, nullptr);
}


dom::nodes::node*
dom::helpers::mutation_algorithms::replace(
        nodes::node* node,
        nodes::node* parent,
        nodes::node* child) {

    if (dynamic_cast<nodes::document*>(parent)) {
        if (dynamic_cast<nodes::document_fragment*>(node)) {
            exceptions::throw_v8_exception(
                    "document_fragment node with a document parent can not have > 1 element child",
                    HIERARCHY_REQUEST_ERR,
                    [node] {return node->child_nodes->filter(&trees::is_element_node).length() > 1;});

            exceptions::throw_v8_exception(
                    "document_fragment node with a document parent can not have > 0 text child",
                    HIERARCHY_REQUEST_ERR,
                    [node] {return node->child_nodes->filter(&trees::is_text_node).length() > 1;});

            if (node->child_nodes->filter(&trees::is_element_node).length() == 1) {

                exceptions::throw_v8_exception(
                        "document_fragment node with a document parent and element child can not have an siblings that != 'child'",
                        HIERARCHY_REQUEST_ERR,
                        [parent, child] {return not parent->child_nodes->filter(&trees::is_element_node).remove(child).empty();});

                exceptions::throw_v8_exception(
                        "document_fragment node with a document parent and element child can not have a document_type child following 'child'",
                        HIERARCHY_REQUEST_ERR,
                        [child] {return not trees::all_following<nodes::document_type*>(child).empty();});
            }
        }

        else if (dynamic_cast<nodes::element*>(node)) {

            exceptions::throw_v8_exception(
                    "element node with a document parent can not have an element child that != 'child'",
                    HIERARCHY_REQUEST_ERR,
                    [node, child] {return not node->child_nodes->filter(&trees::is_element_node).remove(child).empty();;});

            exceptions::throw_v8_exception(
                    "element node with a document parent can not be inserted before a document type node",
                    HIERARCHY_REQUEST_ERR,
                    [child] {return not trees::all_following<nodes::document_type*>(child).empty();});
        }

        else if (dynamic_cast<nodes::document_type*>(node)) {/* TODO */}
    }

    nodes::node* next_sibling = child->next_sibling == node ? node->next_sibling : child->next_sibling;
    nodes::node* previous_sibling = child->previous_sibling;

    ext::vector<nodes::node*> added_nodes = dynamic_cast<nodes::document_fragment*>(node) ? *node->child_nodes : ext::vector<nodes::node*>{node};
    ext::vector<nodes::node*> removed_nodes {};

    if (child->parent) {
        removed_nodes = {child};
        remove(child, true);
    }

    insert(node, parent, next_sibling, true);
    mutation_observers::queue_tree_mutation_record(parent, added_nodes, removed_nodes, previous_sibling, next_sibling);
    return child;
}


dom::nodes::node*
dom::helpers::mutation_algorithms::remove(
        nodes::node* node,
        bool suppress_observers_flag) {

    if (not node->parent) return node;

    nodes::node* parent = node->parent;
    auto node_index = trees::index(node);

    ext::vector<ranges::range*> live_ranges = javascript::realms::current_environment().get("live_ranges");
    live_ranges
            .filter([node](auto* range) {return trees::is_descendant(range->start_container, node);})
            .for_each([parent](auto* range) {range->start_container = parent; range->start_offset = node_index;});

    live_ranges
            .filter([node](auto* range) {return trees::is_descendant(range->end_container, node);})
            .for_each([parent](auto* range) {range->end_container = parent; range->end_offset = node_index;});

    live_ranges
            .filter([parent, node_index](auto* range) {return range->start_container == parent and range->start_offset > node_index;})
            .for_each([](auto* range) {range->start_offset -= 1;});

    live_ranges
            .filter([parent, node_index](auto* range) {return range->end_container == parent and range->end_offset > node_index;})
            .for_each([](auto* range) {range->end_offset -= 1;});

    nodes::node* old_previous_sibling = node->previous_sibling;
    nodes::node* old_next_sibling = node->next_sibling;
    parent->child_nodes->remove(node);

    if (shadows::is_assigned(node))
        shadows::assign_slottables(node->assigned_slot);

    if (shadows::is_root_shadow_root(parent) and shadows::is_slot(parent) and parent->m_assigned_nodes->empty())
        shadows::signal_slot_change(parent);

    if (not trees::descendants(node)->filter([](nodes::node* child) {return shadows::is_slot(child);}).empty()) {
        shadows::assign_slottables_for_tree(trees::root(parent));
        shadows::assign_slottables_for_tree(node);
    }

    if (nodes::element* element = dynamic_cast<nodes::element*>(node)
            and custom_elements::is_custom_node(element)
            and shadows::is_connected(parent)) {

        custom_elements::enqueue_custom_element_callback_reaction(element, "disconnectCallback", {});
    }

    /* TODO : shadow including ancestors */

    for (auto* ancestor: trees::ancestors(parent)) {
        for (auto* registered: ancestor->m_registers_observers_list) {
            auto* transient_registered_observer = new internal::transient_registered_observer{};

            transient_registered_observer->observer = registered->observer;
            transient_registered_observer->options = registered->options;
            transient_registered_observer->source = registered;

            if (registered->options.at("subtree"))
                node->m_registered_observer_list->append(transient_registered_observer);
        }
    }

    if (not suppress_observers_flag)
        mutation_observers::queue_tree_mutation_record(parent, {}, {node}, old_previous_sibling, old_next_sibling);
    return node;
}


void
dom::helpers::mutation_algorithms::replace_all(
        nodes::node* node,
        nodes::node* parent) {

    auto added_nodes = dynamic_cast<nodes::document_fragment*>(node) ? *node->child_nodes : ext::vector<nodes::node*>{node}.remove(nullptr);
    auto removed_nodes = ext::vector<nodes::node*>{*parent->child_nodes};
    parent->child_nodes->for_each([](auto* child) {remove(child);});

    if (node)
        insert(node, parent, nullptr, true);

    if (not added_nodes.empty() or not removed_nodes.empty())
        mutation_observers::queue_tree_mutation_record(parent, added_nodes, removed_nodes, nullptr, nullptr);
}
