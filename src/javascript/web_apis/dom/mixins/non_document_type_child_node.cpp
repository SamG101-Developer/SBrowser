#include "non_document_type_child_node.hpp"

#include <dom/helpers/trees.hpp>

#include <dom/nodes/character_data.hpp>
#include <dom/nodes/element.hpp>


template <typename T>
dom::mixins::non_document_type_child_node<T>::non_document_type_child_node()
{
    // set the custom accessors
    previous_element_sibling.get = [this] {return get_previous_element_sibling();};
    next_element_sibling.get = [this] {return get_next_element_sibling();};
}


template <typename T>
dom::nodes::element*
dom::mixins::non_document_type_child_node<T>::get_previous_element_sibling() const
{
    // get the class that this mixin is being mixed into
    auto* base = reinterpret_cast<const T*>(this);

    // return the last node whose index precedes this node's
    return base->child_nodes
            ->cast_all<nodes::element*>()
            .last_match([base](auto* child) {return helpers::trees::index(child) < helpers::trees::index(base);});
}


template <typename T>
dom::nodes::element*
dom::mixins::non_document_type_child_node<T>::get_next_element_sibling() const
{
    // get the class that this mixin is being mixed into
    auto* base = reinterpret_cast<const T*>(this);

    // return the first node whose index succeeds this node's
    return base->child_nodes
            ->cast_all<nodes::element*>()
            .first_match([base](auto* child) {return helpers::trees::index(child) > helpers::trees::index(base);});
}


template class dom::mixins::non_document_type_child_node<dom::nodes::character_data>;
template class dom::mixins::non_document_type_child_node<dom::nodes::element>;
