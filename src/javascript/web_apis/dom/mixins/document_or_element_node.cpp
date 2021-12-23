#include "document_or_element_node.hpp"

#include <dom/helpers/node_internals.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/element.hpp>


template <typename T>
ext::vector<dom::nodes::element*>
dom::mixins::document_or_element_node<T>::get_elements_by_tag_name(
        ext::cstring& qualified_name) {

    return helpers::node_internals::list_of_elements_with_qualified_name(reinterpret_cast<T*>(this), qualified_name);
}


template <typename T>
ext::vector<dom::nodes::element*>
dom::mixins::document_or_element_node<T>::get_elements_by_tag_name_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name) {

    return helpers::node_internals::list_of_elements_with_namespace_and_local_name(reinterpret_cast<T*>(this), namespace_, local_name);
}


template <typename T>
ext::vector<dom::nodes::element*>
dom::mixins::document_or_element_node<T>::get_elements_by_class_name(
        ext::cstring& class_names) {
    
    return helpers::node_internals::list_of_elements_with_class_names(reinterpret_cast<T*>(this), class_names);
}


template class dom::mixins::document_or_element_node<dom::nodes::element>;
template class dom::mixins::document_or_element_node<dom::nodes::document>;
