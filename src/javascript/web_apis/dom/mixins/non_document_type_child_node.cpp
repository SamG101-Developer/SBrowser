#include "non_document_type_child_node.hpp"

#include <dom/helpers/trees.hpp>

#include <dom/nodes/character_data.hpp>
#include <dom/nodes/element.hpp>


template <typename T>
dom::mixins::non_document_type_child_node<T>::non_document_type_child_node()
{
    // set the custom accessor methods
    bind_get(previous_element_sibling, get_previous_element_sibling);
    bind_get(next_element_sibling, get_next_element_sibling);
}


template <typename T>
auto dom::mixins::non_document_type_child_node<T>::get_previous_element_sibling() const
        -> smart_pointer<nodes::element>
{
    // get the class that this mixin is being mixed into
    auto* base = static_cast<const T*>(this);

    // return the last node whose index precedes this node's
    auto comparison = [base](nodes::node* child) {return helpers::trees::index(child) < helpers::trees::index(base);};
    auto last_match = base->child_nodes->cast_all<nodes::element*>().last_match(comparison);
    return last_match.value_or(nullptr);
}


template <typename T>
auto dom::mixins::non_document_type_child_node<T>::get_next_element_sibling() const
        -> smart_pointer<nodes::element>
{
    // get the class that this mixin is being mixed into
    auto* base = static_cast<const T*>(this);

    // return the first node whose index succeeds this node's
    auto comparison = [base](auto* child) {return helpers::trees::index(child) > helpers::trees::index(base);};
    auto first_match = base->child_nodes->cast_all<nodes::element*>().first_match(comparison);
    return first_match.value_or(nullptr);
}


template <typename T>
auto dom::mixins::non_document_type_child_node<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<non_document_type_child_node<T>>{isolate}
            .template inherit<dom_object>()
            .template var("previousElementSibling", &non_document_type_child_node<T>::previous_element_sibling)
            .template var("nextElementSibling", &non_document_type_child_node<T>::next_element_sibling)
            .auto_wrap_objects();
}


template class dom::mixins::non_document_type_child_node<dom::nodes::character_data>;
template class dom::mixins::non_document_type_child_node<dom::nodes::element>;
