#pragma once
#ifndef SBROWSER_NODE_ITERATOR_HPP
#define SBROWSER_NODE_ITERATOR_HPP

#include <dom/iterators/abstract_iterator.hpp>

namespace dom {
    namespace iterators {class node_iterator;}
    namespace nodes {class node;}
}


class dom::iterators::node_iterator : public abstract_iterator {
public: friends
    friend struct helpers::traversal;

public: constructors
    node_iterator();

public: methods
    auto next_node() -> nodes::node*;
    auto previous_node() -> nodes::node*;

public: properties
    ext::dom_property<nodes::node*, _F> reference_node;
    ext::dom_property<bool, _F> pointer_before_reference_node;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private: internal_properties
    ext::vector<nodes::node*> iterator_collection;
};


#endif //SBROWSER_NODE_ITERATOR_HPP
