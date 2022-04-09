#include "traversal.hpp"

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/iterators/tree_walker.hpp>
#include <dom/iterators/node_iterator.hpp>
#include <dom/iterators/node_filter.hpp>

#include <dom/nodes/node.hpp>


auto dom::helpers::traversal::filter(
        const nodes::node* const node,
        iterators::abstract_iterator* const iterator)
        -> ushort
{
    exceptions::throw_v8_exception<INVALID_STATE_ERR>(
            "an iterator must be inactive in order to be filtered",
            [iterator] {return iterator->m_active_flag;});

    ushort n = node->node_type - (ushort)1;
    if (iterator->what_to_show &~ (ulong)(2 << n))
        return iterators::node_filter::FILTER_SKIP;

    iterator->m_active_flag = true;
//    auto result = webidl::ecma_script_binding::callback_interfaces::call_user_object_operation(iterator->filter, "acceptNode", ext::vector<nodes::node*>{node});
    iterator->m_active_flag = false;

//    return result;
    return 0;
}


auto dom::helpers::traversal::traverse(
        iterators::node_iterator* const iterator,
        const traversal_direction direction)
        -> dom::nodes::node*
{
    // get the current reference node, if there is a pointer before the reference node, and default the result to reject
    nodes::node* node = iterator->reference_node;
    bool before_node = iterator->pointer_before_reference_node;
    auto result = iterators::node_filter::FILTER_REJECT;

    // loop while the result is not to accept the node as the next traversal object
    while (result != iterators::node_filter::FILTER_ACCEPT)
    {
        // handle for a forwards traversal
        if (direction == traversal_direction::NEXT)
        {
            // get the first node following the node if before_node, otherwise just the node
            node = not before_node
                    ? iterator->iterator_collection.first_match([node](const nodes::node* const following_node) {return trees::is_following(node, following_node);})
                    : node;

            // return nullptr if there is no node to traverse to
            if (not node)
                return nullptr;

            // set the before_node option to false
            before_node = false;
        }

        // handle for a backwards traversal
        else if (direction == traversal_direction::PREVIOUS)
        {
            // get the last node preceding the node if before_node, otherwise just the node
            node = before_node
                    ? iterator->iterator_collection.last_match([node](const nodes::node* const preceding_node) {return trees::is_preceding(node, preceding_node);})
                    : node;

            // return nullptr if there is no node to traverse to
            if (not node)
                return nullptr;

            // set the before_node option to true
            before_node = true;
        }

        // get the result of filtering the traversed node against the iterator (ie check that the next/prev node matches
        // a specific filter such as only text nodes etc)
        result = filter(node, iterator);
    }

    // change the current reference node to the next/prev node that has been iterated to
    iterator->reference_node = node;
    iterator->pointer_before_reference_node = before_node;
    return node;
}


auto dom::helpers::traversal::traverse_children(
        iterators::tree_walker* const iterator,
        const traversal_child type)
        -> dom::nodes::node*
{
    // default continue_traversing to true, and set the current node to the correct child depending on the option
    bool continue_traversing = true;
    nodes::node* node = type == traversal_child::FIRST_CHILD
            ? iterator->current_node->first_child
            : iterator->current_node->last_child;

    // continue to loop while the current node isn't nullptr
    while (node) {

        // get the result of filtering the node against the iterator (first time is the child decided above)
        const auto result = filter(node, iterator);

        // handle the case where the result is to accept the node
        if (result == iterators::node_filter::FILTER_ACCEPT)
            return iterator->current_node = node;


        // handle the case where the result is to skip the node
        else if (result == iterators::node_filter::FILTER_SKIP)
        {
            // set the child to either the first or last child of the iterator's current node depending on the option
            if (nodes::node* const child = type == traversal_child::FIRST_CHILD ? iterator->current_node->first_child : iterator->current_node->last_child)
            {
                // update the node to the traversed child, and make sure the second loop doesn't begin by unsetting the
                // continue_traversing flag
                node = child;
                continue_traversing = false;
            }
        }

        // iterate the while loop given there is a node and the continue_traversing flag is still set - this loop is
        // only entered when no child was found and accepted
        while (node and continue_traversing)
        {
            // set the sibling to either the next or previous sibling of the iterator's current node depending on the
            // option
            if (nodes::node* const sibling = type == traversal_child::FIRST_CHILD ? node->next_sibling : node->previous_sibling)
            {
                // update the node to the traversed sibling, and make sure this loop doesn't continue by unsetting the
                // continue_traversing flag (don't break, as the loop has to finish)
                node = sibling;
                continue_traversing = false;
            }

            // if the node/sibling's parent is nullptr, the iterator's root or the iterator's current node, then return
            // nullptr
            if (ext::vector<nodes::node*>{nullptr, iterator->root, iterator->current_node}.contains(node->parent))
                return nullptr;

            // update the node to it's parent
            node = node->parent;
        }
    }

    return nullptr;
}



auto dom::helpers::traversal::traverse_siblings(
        iterators::tree_walker* const iterator,
        const traversal_sibling type)
        -> dom::nodes::node*
{
    // get the iterator's current node, and return nullptr if the iterator's current node is the root (no siblings)
    nodes::node* node = iterator->current_node;
    if (node == iterator->root)
        return nullptr;

    // continue to loop until a return statement is hit in the code block
    while (true)
    {
        // set the sibling to either the next or previous sibling of the iterator's current node depending on the option
        nodes::node* sibling = type == traversal_sibling::NEXT_SIBLING ? node->next_sibling : node->previous_sibling;
        while (sibling)
        {
            // set the node to the sibling, filter it against the iterator (first time is
            // the child decided above)
            node = sibling;
            const auto result = filter(node, iterator);

            // handle the case where the result is to accept the node
            if (result == iterators::node_filter::FILTER_ACCEPT)
                return iterator->current_node = node;

            // set the sibling to either the next or previous sibling of the node
            sibling = type == traversal_sibling::NEXT_SIBLING ? node->first_child : node->last_child;

            // handle the case where the result is to reject the node, or there is no sibling
            if (result == iterators::node_filter::FILTER_REJECT or not sibling)
                sibling = type == traversal_sibling::NEXT_SIBLING ? node->next_sibling : node->previous_sibling;
        }

        // if sibling is nullptr (such that the while loop has exited and the node is the first/last sibling), then set
        // the node to the nodes parent
        node = node->parent;

        // return nullptr if the new node is a nullptr or the root of the iterator
        if (ext::vector<nodes::node*>{nullptr, iterator->root}.contains(node))
            return nullptr;

        // return nullptr if the filtering of the new node is accepted
        if (filter(node, iterator) == iterators::node_filter::FILTER_ACCEPT)
            return nullptr;
    }
}


auto dom::helpers::traversal::traverse_node_previous(
        iterators::tree_walker* const iterator)
        -> dom::nodes::node*
{
    // get the iterator's current node, and default the result to accept
    nodes::node* node = iterator->current_node;
    auto result = iterators::node_filter::FILTER_ACCEPT;

    // loop white the node is not the root of the iterator
    while (node != iterator->root)
    {
        // get the previous sibling
        nodes::node* sibling = node->previous_sibling;

        // loop while the sibling exists
        while (sibling) {

            // set the node to the sibling, and filter it against the iterator
            node = sibling;
            result = filter(node, iterator);

            // loop while the result is not a rejection, and the node has child nodes, setting the node to it's last
            // child and filtering it against the iterator
            while (result != iterators::node_filter::FILTER_REJECT and node->has_child_nodes())
                result = filter(node = node->last_child, iterator);

            // handle the case where the result is to accept the node, by setting the iterator's current node to the
            // node, and returning it
            if (result == iterators::node_filter::FILTER_ACCEPT)
                return iterator->current_node = node;

            // set the sibling to the node's previous sibling and continue the loop
            sibling = node->previous_sibling;
        }

        // return nullptr if the node is the iterator's root, or the node has no parent (set the node to its parent)
        if (node == iterator->root or not (node = node->parent))
            return nullptr;

        // set the iterator's current node to the node and return it
        if (filter(node, iterator) == iterators::node_filter::FILTER_ACCEPT)
            return iterator->current_node = node;
    }

    // return nullptr if the root of the node is reached in previous node traversal
    return nullptr;
}


auto dom::helpers::traversal::traverse_node_next(
        iterators::tree_walker* const iterator)
        -> dom::nodes::node*
{
    // get the iterator's current node, and default the result to accept
    nodes::node* node = iterator->current_node;
    auto result = iterators::node_filter::FILTER_ACCEPT;

    // loop continuously
    while (true) {

        // loop while the result is not a rejection
        while (result != iterators::node_filter::FILTER_REJECT)
        {
            // set the node to its first child, and filter it against the iterator
            node = node->first_child;
            result = filter(node, iterator);

            // handle the case where the result is to accept the node, by setting the iterator's current node to the
            // node, and returning it
            if (result == iterators::node_filter::FILTER_ACCEPT)
                return iterator->current_node = node;
        }

        // create a temporary variable pointing to the same value as the node
        const auto* temporary = node;

        // loop while the temporary exists
        while (temporary)
        {
            // return nullptr if the temporary is the iterator's root
            if (temporary == iterator->root)
                return nullptr;

            // set the node to the temporary's next sibling if it exists, otherwise set the temporary to its parent, ie
            // increment the temporary to the next sibling, until the final sibling is reached, and then move to the
            // parent and repeat, checking that the iterator's root is never reached
            temporary->next_sibling ? node = temporary->next_sibling : temporary = temporary->parent;
        }

        // set the iterator's current node to the node and return it if the result of filtering the node against the
        // iterator is accepted
        if ((result = filter(node, iterator)) == iterators::node_filter::FILTER_ACCEPT)
            return iterator->current_node = node;
    }
}
