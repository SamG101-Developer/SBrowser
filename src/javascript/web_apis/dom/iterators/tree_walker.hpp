#pragma once
#ifndef SBROWSER_TREE_WALKER_HPP
#define SBROWSER_TREE_WALKER_HPP

#include <dom/iterators/abstract_iterator.hpp>

namespace dom::iterators {class tree_walker;}


class dom::iterators::tree_walker final : public abstract_iterator
{
public constructors:
    tree_walker();

public js_methods:
    auto parent_node() -> nodes::node*;
    auto first_child() -> nodes::node*;
    auto last_child() -> nodes::node*;
    auto previous_sibling() -> nodes::node*;
    auto next_sibling() -> nodes::node*;
    auto previous_node() -> nodes::node*;
    auto next_node() -> nodes::node*;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

public js_properties:
    ext::property<smart_pointer<nodes::node>> current_node;
};


#endif //SBROWSER_TREE_WALKER_HPP
