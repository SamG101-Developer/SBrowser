#pragma once
#ifndef SBROWSER_XPATH_RESULT_HPP
#define SBROWSER_XPATH_RESULT_HPP

#include <ext/decorators.hpp>
#include <ext/dom_property.hpp>
#include <dom_object.hpp>

namespace dom
{
    namespace nodes {class node;}
    namespace xpath {class xpath_result;}
}


class dom::xpath::xpath_result : public virtual dom_object
{
public: constructors
    xpath_result();

public: js_static_constants
    static const ushort ANY_TYPE = 0;
    static const ushort NUMBER_TYPE = 1;
    static const ushort STRING_TYPE = 2;
    static const ushort BOOLEAN_TYPE = 3;
    static const ushort UNORDERED_NODE_ITERATOR_TYPE = 4;
    static const ushort ORDERED_NODE_ITERATOR_TYPE = 5;
    static const ushort UNORDERED_NODE_SNAPSHOT_TYPE = 6;
    static const ushort ORDERED_NODE_SNAPSHOT_TYPE = 7;
    static const ushort ANY_UNORDERED_NODE_TYPE = 8;
    static const ushort FIRST_ORDERED_NODE_TYPE = 9;

public: js_methods
    auto iterate_next() -> nodes::node*;
    auto snapshot_item(ulong index) -> nodes::node*;

public: js_properties
    ext::dom_property<ushort> result_type;
    ext::dom_property<double> number_value;
    ext::dom_property<ext::string> string_value;
    ext::dom_property<bool> boolean_value;
    ext::dom_property<nodes::node*> single_node_value;
    ext::dom_property<bool> invalid_iterator_state;
    ext::dom_property<ulong> snapshot_length;

public: cpp_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_XPATH_RESULT_HPP
