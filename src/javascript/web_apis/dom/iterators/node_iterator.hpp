#pragma once
#ifndef SBROWSER_NODE_ITERATOR_HPP
#define SBROWSER_NODE_ITERATOR_HPP

#include <dom/iterators/abstract_iterator.hpp>

namespace dom::iterators {class node_iterator;}
namespace dom::nodes {class node;}


class dom::iterators::node_iterator final : public abstract_iterator
{
public friends:
    friend struct helpers::traversal;

public constructors:
    node_iterator();

public js_methods:
    auto next_node() -> nodes::node*;
    auto previous_node() -> nodes::node*;

public js_properties:
    ext::property<smart_pointer<nodes::node>> reference_node;
    ext::property<bool> pointer_before_reference_node;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    ext::vector<nodes::node*> iterator_collection;
};


#endif //SBROWSER_NODE_ITERATOR_HPP
