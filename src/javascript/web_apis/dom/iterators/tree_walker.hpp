#pragma once
#ifndef SBROWSER_TREE_WALKER_HPP
#define SBROWSER_TREE_WALKER_HPP

#include <dom/iterators/abstract_iterator.hpp>

namespace dom::iterators {class tree_walker;}


/*
 * https://dom.spec.whatwg.org/#interface-treewalker
 * https://developer.mozilla.org/en-US/docs/Web/API/TreeWalker
 *
 * The TreeWalker object represents the nodes of a document subtree and a position within them. A TreeWalker can be
 * created using the Document.createTreeWalker() method.
 */
class dom::iterators::tree_walker : public abstract_iterator
{
public: constructors
    tree_walker() = default;

public: methods
    auto parent_node() -> nodes::node*;
    auto first_child() -> nodes::node*;
    auto last_child() -> nodes::node*;
    auto previous_sibling() -> nodes::node*;
    auto next_sibling() -> nodes::node*;
    auto previous_node() -> nodes::node*;
    auto next_node() -> nodes::node*;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;

public: properties
    ext::dom_property<nodes::node*, _F> current_node;
};


#endif //SBROWSER_TREE_WALKER_HPP
