#include "non_element_parent_node.hpp"

#include <dom/helpers/trees.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/element.hpp>


template<typename T>
auto dom::mixins::non_element_parent_node<T>::get_element_by_id(ext::cstring& element_id) -> dom::nodes::element*
{
    // return the first descendant element whose id matches requested id
    return helpers::trees::descendants(reinterpret_cast<nodes::node*>(this))
            .template cast_all<nodes::element*>()
            .first_match([element_id](nodes::element* child_element) {return child_element->id == element_id;});
}


template <typename T>
auto dom::mixins::non_element_parent_node<T>::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<non_element_parent_node<dom::nodes::node>>{isolate}
            .function("getElementById", &non_element_parent_node<dom::nodes::node>::get_element_by_id)
            .auto_wrap_objects();
}


template class dom::mixins::non_element_parent_node<dom::nodes::document>;
template class dom::mixins::non_element_parent_node<dom::nodes::document_fragment>;
