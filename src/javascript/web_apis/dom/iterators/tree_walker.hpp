#pragma once
#ifndef SBROWSER_TREE_WALKER_HPP
#define SBROWSER_TREE_WALKER_HPP

#include <dom/iterators/abstract_iterator.hpp>

namespace dom::iterators {class tree_walker;}


class dom::iterators::tree_walker : public abstract_iterator{
public: constructors
    tree_walker();

public: methods
    nodes::node* parent_node();
    nodes::node* first_child();
    nodes::node* last_child();
    nodes::node* previous_sibling();
    nodes::node* next_sibling();
    nodes::node* previous_node();
    nodes::node* next_node();

public: internal_methods
    ext::any && v8(v8::Isolate *isolate) const override;

public: properties
    ext::dom_property<nodes::node*> current_node;
};


#endif //SBROWSER_TREE_WALKER_HPP
