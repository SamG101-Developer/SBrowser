#include "node_iterator.hpp"


dom::iterators::node_iterator::node_iterator() = default;


dom::nodes::node*
dom::iterators::node_iterator::next_node() {

    return helpers::traversal::traverse(this, NEXT);
}


dom::nodes::node*
dom::iterators::node_iterator::previous_node() {

    return helpers::traversal::traverse(this, PREVIOUS)
}
