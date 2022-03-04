#include "node_iterator.hpp"

#include <dom/helpers/traversal.hpp>


dom::iterators::node_iterator::node_iterator() = default;


/*
 * https://dom.spec.whatwg.org/#dom-nodeiterator-nextnode
 * https://developer.mozilla.org/en-US/docs/Web/API/NodeIterator/nextNode
 *
 * The NodeIterator.nextNode() method returns the next node in the set represented by the NodeIterator and advances the
 * position of the iterator within the set. The first call to nextNode() returns the first node in the set.
 *
 * This method returns null when there are no nodes left in the set. In old browsers, as specified in old versions of
 * the specifications, the method may throws the INVALID_STATE_ERR DOMException if this method is called after the
 * NodeIterator.detach()method. Recent browsers never throw.
 */
auto dom::iterators::node_iterator::next_node() -> dom::nodes::node*
{
    // get the next node in the iterator by calling the traverse helper method
    return helpers::traversal::traverse(this, traversal_direction::NEXT);
}


/*
 * https://dom.spec.whatwg.org/#dom-nodeiterator-previousnode
 * https://developer.mozilla.org/en-US/docs/Web/API/NodeIterator/previousNode
 *
 * The NodeIterator.previousNode() method returns the previous node in the set represented by the NodeIterator and moves
 * the position of the iterator backwards within the set.
 *
 * This method returns null when the current node is the first node in the set. In old browsers, as specified in old
 * versions of the specifications, the method may throws the INVALID_STATE_ERR DOMException if this method is called
 * after the NodeIterator.detach()method. Recent browsers never throw.
 */
auto dom::iterators::node_iterator::previous_node() -> dom::nodes::node*
{
    // get the previous node in the iterator by calling the traverse helper method
    return helpers::traversal::traverse(this, traversal_direction::PREVIOUS);
}


auto dom::iterators::node_iterator::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<node_iterator>{isolate}
            .inherit<abstract_iterator>()
            .var("referenceNode", &node_iterator::reference_node)
            .var("pointerBeforeReferenceNode", &node_iterator::pointer_before_reference_node)
            .function("nextNode", &node_iterator::next_node)
            .function("previousNode", &node_iterator::previous_node)
            .auto_wrap_objects();
}
