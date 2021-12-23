#include "non_element_parent_node.hpp"

#include <dom/helpers/trees.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/element.hpp>


template<typename T>
dom::nodes::element*
dom::mixins::non_element_parent_node<T>::get_element_by_id(
        ext::cstring& element_id) {

    return helpers::trees::descendants(reinterpret_cast<nodes::node*>(this))
            .template cast_all<nodes::element*>()
            .filter([element_id](nodes::element* child_element) {return child_element->id == element_id;})
            .front();
}


template class dom::mixins::non_element_parent_node<dom::nodes::document>;
template class dom::mixins::non_element_parent_node<dom::nodes::document_fragment>;
