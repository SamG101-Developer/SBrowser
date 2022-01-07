#include "node_iterator.hpp"

#include <dom/helpers/traversal.hpp>


dom::iterators::node_iterator::node_iterator() = default;


dom::nodes::node*
dom::iterators::node_iterator::next_node() {

    return helpers::traversal::traverse(this, NEXT);
}


dom::nodes::node*
dom::iterators::node_iterator::previous_node() {

    return helpers::traversal::traverse(this, PREVIOUS);
}


ext::any&&
dom::iterators::node_iterator::v8(v8::Isolate* isolate) const {

    return v8pp::class_<node_iterator>{isolate}
            .inherit<abstract_iterator>()

            .var("referenceNode", &node_iterator::reference_node)
            .var("pointerBeforeReferenceNode", &node_iterator::pointer_before_reference_node)

            .function("nextNode", &node_iterator::next_node)
            .function("previousNode", &node_iterator::previous_node)

            .auto_wrap_objects();
}
