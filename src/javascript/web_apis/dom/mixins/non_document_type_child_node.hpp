#pragma once
#ifndef SBROWSER_NON_DOCUMENT_TYPE_CHILD_NODE_HPP
#define SBROWSER_NON_DOCUMENT_TYPE_CHILD_NODE_HPP

#include <ext/properties/dom_property.hpp>

namespace dom {
    namespace mixins {template <typename T> class non_document_type_child_node;}
    namespace nodes {class element;}
}


template <typename T>
class dom::mixins::non_document_type_child_node {

public: constructors
    non_document_type_child_node();

public: properties
    ext::dom_property<nodes::element*, _F> previous_element_sibling;
    ext::dom_property<nodes::element*, _F> next_element_sibling;

private: accessors
    nodes::element* get_previous_element_sibling() const;
    nodes::element* get_next_element_sibling() const;
};


#endif //SBROWSER_NON_DOCUMENT_TYPE_CHILD_NODE_HPP
