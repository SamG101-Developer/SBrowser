#include "tree_walker.hpp"

#include <dom/helpers/traversal.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/iterators/node_filter.hpp>


dom::iterators::tree_walker::tree_walker() = default;


dom::nodes::node*
dom::iterators::tree_walker::parent_node() {
    current_node = helpers::trees::ancestors(current_node)
            .filter([](auto* node) {return helpers::traversal::filter(node, this) == node_filter::FILTER_ACCEPT;})
            .front();

    return current_node;
}


dom::nodes::node*
dom::iterators::tree_walker::first_child() {

    return helpers::traversal::traverse_children(this, FIRST_CHILD);
}


dom::nodes::node*
dom::iterators::tree_walker::last_child() {

    return helpers::traversal::traverse_children(this, LAST_CHILD);
}


dom::nodes::node*
dom::iterators::tree_walker::next_sibling() {

    return helpers::traversal::traverse_siblings(this, NEXT_SIBLING);
}


dom::nodes::node*
dom::iterators::tree_walker::previous_sibling() {

    return helpers::traversal::traverse_siblings(this, PREVIOUS_SIBLING);
}


dom::nodes::node*
dom::iterators::tree_walker::next_node() {

    return helpers::traversal::traverse_node_next(this);
}


dom::nodes::node*
dom::iterators::tree_walker::previous_node() {

    return helpers::traversal::traverse_node_previous(this);
}


ext::any
dom::iterators::tree_walker::v8(v8::Isolate* isolate) const {

    return v8pp::class_<tree_walker>{isolate}
            .inherit<dom::iterators::abstract_iterator>()

            .function("parentNode", &dom::iterators::tree_walker::parent_node)
            .function("firstChild", &dom::iterators::tree_walker::first_child)
            .function("lastChild", &dom::iterators::tree_walker::last_child)
            .function("previousSibling", &dom::iterators::tree_walker::previous_sibling)
            .function("nextSibling", &dom::iterators::tree_walker::next_sibling)
            .function("previousNode", &dom::iterators::tree_walker::previous_node)
            .function("nextNode", &dom::iterators::tree_walker::next_node)

            .var("currentNode", &dom::iterators::tree_walker::current_node)

            .auto_wrap_objects();
}
