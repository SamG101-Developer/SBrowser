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
auto dom::mixins::non_document_type_child_node<T>::get_previous_element_sibling() const -> dom::nodes::element*
{
    // get the class that this mixin is being mixed into
    const nodes::node* base = static_cast<const T*>(this);

    // return the last node whose index precedes this node's
    return base->child_nodes
            ->template cast_all<nodes::element*>()
            .last_match([base](auto* child) {return helpers::trees::index(child) < helpers::trees::index(base);});
}


template <typename T>
auto dom::mixins::non_document_type_child_node<T>::get_next_element_sibling() const -> dom::nodes::element*
{
    // get the class that this mixin is being mixed into
    const nodes::node* base = static_cast<const T*>(this);

    // return the first node whose index succeeds this node's
    return base->child_nodes
            ->template cast_all<nodes::element*>()
            .first_match([base](auto* child) {return helpers::trees::index(child) > helpers::trees::index(base);});
}


template <typename T>
auto dom::mixins::non_document_type_child_node<T>::v8(v8::Isolate* isolate) const -> ext::any {
    return v8pp::class_<dom::mixins::non_document_type_child_node<T>>{isolate}
            .var("previousElementSibling", &dom::mixins::non_document_type_child_node<T>::previous_element_sibling)
            .var("nextElementSibling", &dom::mixins::non_document_type_child_node<T>::next_element_sibling)
            .auto_wrap_objects();
}


template class dom::mixins::non_document_type_child_node<dom::nodes::character_data>;
template class dom::mixins::non_document_type_child_node<dom::nodes::element>;
