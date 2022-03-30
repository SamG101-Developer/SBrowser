#pragma once
#ifndef SBROWSER_NON_ELEMENT_PARENT_NODE_HPP
#define SBROWSER_NON_ELEMENT_PARENT_NODE_HPP

#include <ext/string.hpp>
#include <dom_object.hpp>

namespace dom::mixins {template <typename T> class non_element_parent_node;}
namespace dom::nodes {class element;}


template <typename T>
class dom::mixins::non_element_parent_node : public dom_object
{
public js_methods:
    auto get_element_by_id(const ext::string& element_id) -> nodes::element*;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_NON_ELEMENT_PARENT_NODE_HPP
