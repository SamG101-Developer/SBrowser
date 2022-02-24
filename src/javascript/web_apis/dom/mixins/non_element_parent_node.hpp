#pragma once
#ifndef SBROWSER_NON_ELEMENT_PARENT_NODE_HPP
#define SBROWSER_NON_ELEMENT_PARENT_NODE_HPP

#include <ext/iterables/string.hpp>
#include <dom_object.hpp>

namespace dom
{
    namespace mixins {template <typename T> class non_element_parent_node;}
    namespace nodes {class element;}
}


template <typename T>
class dom::mixins::non_element_parent_node : public dom_object
{
public: methods
    auto get_element_by_id(ext::cstring& element_id) -> nodes::element*;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_NON_ELEMENT_PARENT_NODE_HPP
