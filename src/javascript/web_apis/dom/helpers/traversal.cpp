#include "traversal.hpp"

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/iterators/tree_walker.hpp>
#include <dom/iterators/node_iterator.hpp>
#include <dom/iterators/node_filter.hpp>

#include <dom/nodes/node.hpp>


unsigned short
dom::helpers::traversal::filter(
        nodes::node* node,
        iterators::abstract_iterator* iterator) {

    exceptions::throw_v8_exception(
            "an iterator must be inactive in order to be filtered",
            INVALID_STATE_ERR,
            [iterator] {return iterator->m_active_flag;});

    auto n = node->node_type - 1;
    if (iterator->what_to_show &~ (2 << n))
        return iterators::node_filter::FILTER_SKIP;

    iterator->m_active_flag = true;
    auto result = webidl::ecma_script_binding::callback_interfaces::call_user_object_operation(iterator->filter, "acceptNode", ext::vector<nodes::node*>{node});
    iterator->m_active_flag = false;

    return result;
}


dom::nodes::node*
dom::helpers::traversal::traverse(
        iterators::node_iterator* iterator,
        traversal_direction direction) {

    nodes::node* node = iterator->reference_node;
    bool before_node = iterator->pointer_before_reference_node;
    auto result = iterators::node_filter::FILTER_REJECT;

    while (result != iterators::node_filter::FILTER_ACCEPT) {
        if (direction == traversal_direction::NEXT) {
            node = not before_node
                    ? iterator->iterator_collection.first_match([node](auto* following_node) {return trees::is_following(node, following_node);})
                    : node;

            if (before_node) return nullptr;
            before_node = false;
        }
        else if (direction == traversal_direction::PREVIOUS) {
            node = before_node
                    ? iterator->iterator_collection.last_match([node](auto* preceding_node) {return trees::is_preceding(node, preceding_node);})
                    : node;

            if (not before_node) return nullptr;
            before_node = true;
        }

        result = filter(node, iterator);
    }

    iterator->reference_node = node;
    iterator->pointer_before_reference_node = before_node;
    return node;
}


dom::nodes::node*
dom::helpers::traversal::traverse_children(
        iterators::tree_walker* iterator,
        traversal_child type) {

    bool continue_traversing = true;
    nodes::node* node = type == FIRST_CHILD
            ? iterator->current_node->first_child
            : iterator->current_node->last_child;

    while (node) {
        auto result = filter(node, iterator);

        if (result == iterators::node_filter::FILTER_ACCEPT)
            return iterator->current_node = node;

        else if (result == iterators::node_filter::FILTER_SKIP) {
            nodes::node* child = type == FIRST_CHILD
                    ? iterator->current_node->first_child
                    : iterator->current_node->last_child;

            if (child)
                node = child, continue_traversing = false;
        }

        while (node and continue_traversing) {

            nodes::node* sibling = type == FIRST_CHILD
                    ? node->next_sibling
                    : node->previous_sibling;

            if (sibling)
                node = sibling, continue_traversing = false;

            nodes::node* parent = node->parent_node;
            if (ext::vector<nodes::node*>{nullptr, iterator->root, iterator->current_node}.contains(parent))
                return nullptr;

            node = parent;
        }
    }

    return nullptr;
}


dom::nodes::node*
dom::helpers::traversal::traverse_siblings(
        iterators::tree_walker* iterator,
        traversal_sibling type) {

    nodes::node* node = iterator->current_node;
    if (node == iterator->root)
        return nullptr;

    while (true) {

        nodes::node* sibling = type == NEXT_SIBLING
                ? node->next_sibling
                : node->previous_sibling;

        while (sibling) {
            node = sibling;
            auto result = filter(node, iterator);

            if (result == iterators::node_filter::FILTER_ACCEPT)
                return iterator->current_node = node;

            sibling = type == NEXT_SIBLING
                    ? node->first_child
                    : node->last_child;

            if (result == iterators::node_filter::FILTER_REJECT or not sibling)
                sibling = type == NEXT_SIBLING
                        ? node->next_sibling
                        : node->previous_sibling;
        }
    }

    node = node->parent_node;
    if (ext::vector<nodes::node*>{nullptr, iterator->root}.contains(node))
        return nullptr;
    if (filter(node, iterator) == iterators::node_filter::FILTER_ACCEPT)
        return nullptr;
}


dom::nodes::node*
dom::helpers::traversal::traverse_node_previous(
        iterators::tree_walker* iterator) {

    nodes::node* node = iterator->current_node;
    auto result = iterators::node_filter::FILTER_ACCEPT;

    while (node != iterator->root) {
        nodes::node* sibling = node->previous_sibling;
        while (sibling) {
            node = sibling;
            result = filter(node, iterator);

            while (result != iterators::node_filter::FILTER_REJECT and node->has_child_nodes())
                result = filter(node = node->last_child, iterator);

            if (result == iterators::node_filter::FILTER_ACCEPT)
                return iterator->current_node = node;

            sibling = node->previous_sibling;
        }

        if (node == iterator->root or not (node = node->parent_node))
            return nullptr;

        if (filter(node, iterator) == iterators::node_filter::FILTER_ACCEPT)
            return iterator->current_node = node;
    }
    return nullptr;
}


dom::nodes::node*
dom::helpers::traversal::traverse_node_next(
        iterators::tree_walker* iterator) {

    nodes::node* node = iterator->current_node;
    auto result = iterators::node_filter::FILTER_ACCEPT;

    while (true) {
        while (result != iterators::node_filter::FILTER_REJECT) {
            node = node->first_child;
            result = filter(node, iterator);
            if (result == iterators::node_filter::FILTER_ACCEPT)
                return iterator->current_node = node;
        }

        auto* temporary = node;

        while (temporary) {
            if (temporary == iterator->root) return nullptr;
            temporary->next_sibling ? node = temporary->next_sibling : temporary = temporary->parent_node;
        }

        if ((result = filter(node, iterator)) == iterators::node_filter::FILTER_ACCEPT)
            return iterator->current_node = node;
    }
}
