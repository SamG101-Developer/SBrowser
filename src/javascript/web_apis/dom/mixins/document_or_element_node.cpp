#include "document_or_element_node.hpp"

#include <dom/helpers/node_internals.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/element.hpp>


template <typename T>
auto dom::mixins::document_or_element_node<T>::get_elements_by_tag_name(
        const ext::string& qualified_name)
        -> ext::vector<dom::nodes::element*>
{
    // get the class that this mixin is being mixed into, and return descendant elements with a matching qualified name
    auto* base = static_cast<T*>(this);
    return helpers::node_internals::list_of_elements_with_qualified_name(base, qualified_name);
}


template <typename T>
auto dom::mixins::document_or_element_node<T>::get_elements_by_tag_name_ns(
        const ext::string& namespace_,
        const ext::string& local_name)
        -> ext::vector<dom::nodes::element*>
{
    // get the class that this mixin is being mixed into, and return descendant elements with a matching local name
    auto* base = static_cast<T*>(this);
    return helpers::node_internals::list_of_elements_with_namespace_and_local_name(base, namespace_, local_name);
}


template <typename T>
auto dom::mixins::document_or_element_node<T>::get_elements_by_class_name(
        const ext::string& class_names)
        -> ext::vector<dom::nodes::element*>
{
    // get the class that this mixin is being mixed into, and return descendant elements with a matching class names
    auto* base = static_cast<T*>(this);
    return helpers::node_internals::list_of_elements_with_class_names(base, class_names);
}


template <typename T>
auto dom::mixins::document_or_element_node<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<document_or_element_node<dom::nodes::node>>{isolate}
            .inherit<dom_object>()
            .function("getElementsByTagName", &document_or_element_node<T>::get_elements_by_tag_name)
            .function("getElementsByTagNameNS", &document_or_element_node<T>::get_elements_by_tag_name_ns)
            .function("getElementsByClassName", &document_or_element_node<T>::get_elements_by_class_name)
            .auto_wrap_objects();
}


template class dom::mixins::document_or_element_node<dom::nodes::document>;
template class dom::mixins::document_or_element_node<dom::nodes::element>;
