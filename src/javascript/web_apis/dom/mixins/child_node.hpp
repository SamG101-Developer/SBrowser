#pragma once
#ifndef SBROWSER_CHILD_NODE_HPP
#define SBROWSER_CHILD_NODE_HPP

#include <dom_object.hpp>
namespace dom::mixins {template <typename T> class child_node;}

#include <javascript/interop/attribute_descriptors.hpp>


template <typename T>
class dom::mixins::child_node : public virtual dom_object
{
public js_methods:
    unscopable template<typename ...nodes_or_strings_t> auto before(nodes_or_strings_t... nodes) -> void;
    unscopable template<typename ...nodes_or_strings_t> auto after(nodes_or_strings_t... nodes) -> void;
    unscopable template<typename ...nodes_or_strings_t> auto replace_with(nodes_or_strings_t... nodes) -> void;
    unscopable auto remove() -> void;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_CHILD_NODE_HPP
