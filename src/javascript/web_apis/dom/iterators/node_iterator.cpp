#include "node_iterator.hpp"

#include <dom/helpers/traversal.hpp>
#include <dom/nodes/node.hpp>


dom::iterators::node_iterator::node_iterator()
        : pointer_before_reference_node(false)
{
    reference_node = nullptr;
}


auto dom::iterators::node_iterator::next_node() -> dom::nodes::node*
{
    // get the next node in the iterator by calling the traverse helper method
    return helpers::traversal::traverse(this, traversal_direction::NEXT);
}


auto dom::iterators::node_iterator::previous_node() -> dom::nodes::node*
{
    // get the previous node in the iterator by calling the traverse helper method
    return helpers::traversal::traverse(this, traversal_direction::PREVIOUS);
}


auto dom::iterators::node_iterator::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<node_iterator>{isolate}
            .inherit<abstract_iterator>()
            .function("nextNode", &node_iterator::next_node)
            .function("previousNode", &node_iterator::previous_node)
            .var("referenceNode", &node_iterator::reference_node, true)
            .var("pointerBeforeReferenceNode", &node_iterator::pointer_before_reference_node, true)
            .auto_wrap_objects();
}
