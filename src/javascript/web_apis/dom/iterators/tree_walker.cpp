#include "tree_walker.hpp"

#include <dom/helpers/traversal.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/iterators/node_filter.hpp>


dom::iterators::tree_walker::tree_walker() = default;


auto dom::iterators::tree_walker::parent_node() -> nodes::node*
{
    current_node = helpers::trees::ancestors(current_node)
            .filter([this](auto* node) {return helpers::traversal::filter(node, this) == node_filter::FILTER_ACCEPT;})
            .front();

    return current_node;
}


auto dom::iterators::tree_walker::first_child() -> nodes::node*
{
    // get the first child in the iterator by calling the traverse_children helper method
    return helpers::traversal::traverse_children(this, FIRST_CHILD);
}


auto dom::iterators::tree_walker::last_child() -> nodes::node*
{
    // get the last child in the iterator by calling the traverse_children helper method
    return helpers::traversal::traverse_children(this, LAST_CHILD);
}


auto dom::iterators::tree_walker::next_sibling() -> nodes::node*
{
    // get the next sibling in the iterator by calling the traverse_siblings helper method
    return helpers::traversal::traverse_siblings(this, NEXT_SIBLING);
}


auto dom::iterators::tree_walker::previous_sibling() -> nodes::node*
{
    // get the previous sibling in the iterator by calling the traverse_siblings helper method
    return helpers::traversal::traverse_siblings(this, PREVIOUS_SIBLING);
}


auto dom::iterators::tree_walker::next_node() -> nodes::node*
{
    // get the next node in the iterator by calling the traverse_node_next helper method
    return helpers::traversal::traverse_node_next(this);
}


auto dom::iterators::tree_walker::previous_node() -> nodes::node*
{
    // get the previous node in the iterator by calling the traverse_node_previous helper method
    return helpers::traversal::traverse_node_previous(this);
}


auto dom::iterators::tree_walker::v8(v8::Isolate* isolate) const -> ext::any
{
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
