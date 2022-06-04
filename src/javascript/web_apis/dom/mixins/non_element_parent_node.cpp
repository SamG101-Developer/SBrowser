#include "non_element_parent_node.hpp"

#include <dom/helpers/trees.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/element.hpp>


template<typename T>
auto dom::mixins::non_element_parent_node<T>::get_element_by_id(
        const ext::string& element_id)
        -> nodes::element*
{
    // get the class that this mixin is being mixed into
    auto* base = static_cast<T*>(this);

    // return the first descendant element whose id matches requested id
    auto comparison  = [element_id](nodes::element* child_element) {return child_element->id == element_id;};
    auto first_match = helpers::trees::descendants(base).cast_all<nodes::element*>().first_match(comparison);
    return first_match.value_or(nullptr);
}


template <typename T>
auto dom::mixins::non_element_parent_node<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<non_element_parent_node<T>>{isolate}
            .template inherit<dom_object>()
            .template function("getElementById", &non_element_parent_node<T>::get_element_by_id)
            .auto_wrap_objects();
}


template class dom::mixins::non_element_parent_node<dom::nodes::document>;
template class dom::mixins::non_element_parent_node<dom::nodes::document_fragment>;
