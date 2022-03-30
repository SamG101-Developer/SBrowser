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
    // return the first descendant element whose id matches requested id
    return helpers::trees::descendants(static_cast<T*>(this))
            .template cast_all<nodes::element*>()
            .template first_match([element_id](const nodes::element* const child_element) {return child_element->id == element_id;});
}


template <typename T>
auto dom::mixins::non_element_parent_node<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<non_element_parent_node<T>>{isolate}
            .inherit<dom_object>()
            .function("getElementById", &non_element_parent_node<T>::get_element_by_id)
            .auto_wrap_objects();
}


template class dom::mixins::non_element_parent_node<dom::nodes::document>;
template class dom::mixins::non_element_parent_node<dom::nodes::document_fragment>;
