#include "tree_walker.hpp"

#include <dom/helpers/traversal.hpp>
#include <dom/helpers/trees.hpp>
#include <dom/iterators/node_filter.hpp>


/*
 * https://dom.spec.whatwg.org/#dom-treewalker-parentnode
 * https://developer.mozilla.org/en-US/docs/Web/API/TreeWalker/parentNode
 *
 * The TreeWalker.parentNode() method moves the current Node to the first visible ancestor node in the document order,
 * and returns the found node. If no such node exists, or if it is above the TreeWalker's root node, returns null and
 * the current node is not changed.
 */
auto dom::iterators::tree_walker::parent_node() -> nodes::node*
{
    current_node = helpers::trees::ancestors(current_node)
            .filter([this](auto* node) {return helpers::traversal::filter(node, this) == node_filter::FILTER_ACCEPT;})
            .front();

    return current_node;
}


/*
 * https://dom.spec.whatwg.org/#dom-treewalker-parentnode
 * https://developer.mozilla.org/en-US/docs/Web/API/TreeWalker/parentNode
 *
 * The TreeWalker.parentNode() method moves the current Node to the first visible ancestor node in the document order,
 * and returns the found node. If no such node exists, or if it is above the TreeWalker's root node, returns null and
 * the current node is not changed.
 */
auto dom::iterators::tree_walker::first_child() -> nodes::node*
{
    // get the first child in the iterator by calling the traverse_children helper method
    return helpers::traversal::traverse_children(this, traversal_child::FIRST_CHILD);
}


/*
 * https://dom.spec.whatwg.org/#dom-treewalker-firstchild
 * https://developer.mozilla.org/en-US/docs/Web/API/TreeWalker/firstChild
 *
 * The TreeWalker.firstChild() method moves the current Node to the first visible child of the current node, and returns
 * the found child. It also moves the current node to this child. If no such child exists, returns null and the current
 * node is not changed.
 */
auto dom::iterators::tree_walker::last_child() -> nodes::node*
{
    // get the last child in the iterator by calling the traverse_children helper method
    return helpers::traversal::traverse_children(this, traversal_child::LAST_CHILD);
}


/*
 * https://dom.spec.whatwg.org/#dom-treewalker-nextsibling
 * https://developer.mozilla.org/en-US/docs/Web/API/TreeWalker/previousSibling
 *
 * The TreeWalker.nextSibling() method moves the current Node to its next sibling, if any, and returns the found
 * sibling. If there is no such node, return null and the current node is not changed.
 */
auto dom::iterators::tree_walker::next_sibling() -> nodes::node*
{
    // get the next sibling in the iterator by calling the traverse_siblings helper method
    return helpers::traversal::traverse_siblings(this, traversal_sibling::NEXT_SIBLING);
}


/*
 * https://dom.spec.whatwg.org/#dom-treewalker-previoussibling
 * https://developer.mozilla.org/en-US/docs/Web/API/TreeWalker/previousSibling
 *
 * The TreeWalker.previousSibling() method moves the current Node to its previous sibling, if any, and returns the found
 * sibling. If there is no such node, return null and the current node is not changed.
 */
auto dom::iterators::tree_walker::previous_sibling() -> nodes::node*
{
    // get the previous sibling in the iterator by calling the traverse_siblings helper method
    return helpers::traversal::traverse_siblings(this, traversal_sibling::PREVIOUS_SIBLING);
}


/*
 * https://dom.spec.whatwg.org/#dom-treewalker-nextnode
 * https://developer.mozilla.org/en-US/docs/Web/API/TreeWalker/nextNode
 *
 * The TreeWalker.nextNode() method moves the current Node to the next visible node in the document order, and returns
 * the found node. It also moves the current node to this one. If no such node exists, returns null and the current node
 * is not changed.
 */
auto dom::iterators::tree_walker::next_node() -> nodes::node*
{
    // get the next node in the iterator by calling the traverse_node_next helper method
    return helpers::traversal::traverse_node_next(this);
}


/*
 * https://dom.spec.whatwg.org/#dom-treewalker-previousnode
 * https://developer.mozilla.org/en-US/docs/Web/API/TreeWalker/previousNode
 *
 * The TreeWalker.previousNode() method moves the current Node to the previous visible node in the document order, and
 * returns the found node. It also moves the current node to this one. If no such node exists, or if it is before that
 * the root node defined at the object construction, returns null and the current node is not changed.
 */
auto dom::iterators::tree_walker::previous_node() -> nodes::node*
{
    // get the previous node in the iterator by calling the traverse_node_previous helper method
    return helpers::traversal::traverse_node_previous(this);
}


auto dom::iterators::tree_walker::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<tree_walker>{isolate}
            .inherit<abstract_iterator>()
            .function("parentNode", &tree_walker::parent_node)
            .function("firstChild", &tree_walker::first_child)
            .function("lastChild", &tree_walker::last_child)
            .function("previousSibling", &tree_walker::previous_sibling)
            .function("nextSibling", &tree_walker::next_sibling)
            .function("previousNode", &tree_walker::previous_node)
            .function("nextNode", &tree_walker::next_node)
            .var("currentNode", &tree_walker::current_node)
            .auto_wrap_objects();
}
