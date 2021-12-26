#include "non_document_type_child_node.hpp"

#include <dom/helpers/trees.hpp>

#include <dom/nodes/character_data.hpp>
#include <dom/nodes/element.hpp>


template <typename T>
dom::mixins::non_document_type_child_node<T>::non_document_type_child_node() {
    previous_element_sibling.get = [this] {return get_previous_element_sibling();};
    next_element_sibling.get = [this] {return get_next_element_sibling();};
}


template <typename T>
dom::nodes::element*
dom::mixins::non_document_type_child_node<T>::get_previous_element_sibling() const {
    const T* base = reinterpret_cast<const T*>(this);

    return base->child_nodes
            ->cast_all<nodes::element*>()
            .filter([this](auto* child) {return helpers::trees::index(child) < helpers::trees::index(base);})
            .back();
}


template <typename T>
dom::nodes::element*
dom::mixins::non_document_type_child_node<T>::get_next_element_sibling() const {
    const T* base = reinterpret_cast<const T*>(this);

    return base->child_nodes
            ->cast_all<nodes::element*>()
            .filter([this](auto* child) {return helpers::trees::index(child) > helpers::trees::index(base);})
            .front();
}


template class dom::mixins::non_document_type_child_node<dom::nodes::character_data>;
template class dom::mixins::non_document_type_child_node<dom::nodes::element>;
