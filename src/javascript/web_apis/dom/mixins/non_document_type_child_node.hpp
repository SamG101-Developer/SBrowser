#pragma once
#ifndef SBROWSER_NON_DOCUMENT_TYPE_CHILD_NODE_HPP
#define SBROWSER_NON_DOCUMENT_TYPE_CHILD_NODE_HPP

#include <ext/types/property.hpp>

namespace dom {
    namespace mixins {template <typename T> class non_document_type_child_node;}
    namespace nodes {class element;}
}


template <typename T>
class dom::mixins::non_document_type_child_node {
public:
    non_document_type_child_node();

    ext::dom_property<nodes::element*> previous_element_sibling;
    ext::dom_property<nodes::element*> next_element_sibling;

private:
    nodes::element* get_previous_element_sibling() const;
    nodes::element* get_next_element_sibling() const;
};


#endif //SBROWSER_NON_DOCUMENT_TYPE_CHILD_NODE_HPP
