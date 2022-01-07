#pragma once
#ifndef SBROWSER_NODE_ITERATOR_HPP
#define SBROWSER_NODE_ITERATOR_HPP

#include <dom/iterators/abstract_iterator.hpp>

namespace dom {
    namespace iterators {class node_iterator;}
    namespace nodes {class node;}
}


class dom::iterators::node_iterator : public abstract_iterator {
friends
    friend struct helpers::traversal;

public: constructors
    node_iterator();

public: methods
    nodes::node* next_node();
    nodes::node* previous_node();

public: properties
    ext::dom_property<nodes::node*> reference_node;
    ext::dom_property<bool> pointer_before_reference_node;

public: internal_methods
    ext::any && v8(v8::Isolate *isolate) const override;

private: internal_properties
    ext::vector<nodes::node*> iterator_collection;
};


#endif //SBROWSER_NODE_ITERATOR_HPP
