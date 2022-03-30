#pragma once
#ifndef SBROWSER_NON_DOCUMENT_TYPE_CHILD_NODE_HPP
#define SBROWSER_NON_DOCUMENT_TYPE_CHILD_NODE_HPP

#include <ext/dom_property.hpp>
#include <dom_object.hpp>

namespace dom::mixins {template <typename T> class non_document_type_child_node;}
namespace dom::nodes {class element;}


template <typename T>
class dom::mixins::non_document_type_child_node : public dom_object
{
public constructors:
    non_document_type_child_node();

public js_properties:
    ext::dom_property<nodes::element*> previous_element_sibling;
    ext::dom_property<nodes::element*> next_element_sibling;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private accessors:
    auto get_previous_element_sibling() const -> nodes::element*;
    auto get_next_element_sibling() const -> nodes::element*;
};


#endif //SBROWSER_NON_DOCUMENT_TYPE_CHILD_NODE_HPP
