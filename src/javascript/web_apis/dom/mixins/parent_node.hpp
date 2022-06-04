#pragma once
#ifndef SBROWSER_PARENT_NODE_HPP
#define SBROWSER_PARENT_NODE_HPP

#include <dom_object.hpp>
namespace dom::mixins {template <typename T> class parent_node;}

#include <ext/vector.hpp>
namespace dom::nodes {class element;}
namespace dom::nodes {class node;}


template <typename T>
class dom::mixins::parent_node : public virtual dom_object
{
public constructors:
    parent_node();

public js_methods:
    unscopable template<typename ...nodes_or_strings_t> auto prepend(nodes_or_strings_t... nodes) -> void;
    unscopable template<typename ...nodes_or_strings_t> auto append(nodes_or_strings_t... nodes) -> void;
    unscopable template<typename ...nodes_or_strings_t> auto replace_children(nodes_or_strings_t... nodes) -> void;

    auto query_selector(ext::string selectors) -> nodes::element*;
    auto query_selector_all(ext::string selectors) -> ext::vector<nodes::node*>;

public js_properties:
    ext::property<smart_pointer<ext::vector<smart_pointer<nodes::element>>>> children;
    ext::property<smart_pointer<nodes::element>> first_element_child;
    ext::property<smart_pointer<nodes::element>> last_element_child;
    ext::property<size_t> child_element_count;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private accessors:
    [[nodiscard]] auto get_first_element_child() const -> smart_pointer<nodes::element>;
    [[nodiscard]] auto get_last_element_child() const -> smart_pointer<nodes::element>;
    [[nodiscard]] auto get_child_element_count() const -> size_t;
};


#endif //SBROWSER_PARENT_NODE_HPP
