#include "parent_node.hpp"

#include <dom/nodes/element.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>

#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/node_internals.hpp>


template<typename T>
dom::mixins::parent_node<T>::parent_node()
{
    // set the custom accessor methods
    children.getter = [this] {return get_children();};
    first_element_child.getter = [this] {return get_first_element_child();};
    last_element_child.getter = [this] {return get_last_element_child();};
    child_element_count.getter = [this] {return get_child_element_count();};
}


template <typename T>
template <typename ...nodes_or_strings_t>
auto dom::mixins::parent_node<T>::prepend(
        nodes_or_strings_t ...nodes)
        -> void
{
    // get the class that this mixin is being mixed into, and the node
    auto* base = static_cast<T*>(this);
    auto* node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);

    // pre-insert this node into this node's children before first child
    helpers::mutation_algorithms::pre_insert(node, this, base->child_nodes->front());

    // custom element reactions
    ce_reactions(&parent_node<T>::prepend);
}


template <typename T>
template <typename ...nodes_or_strings_t>
auto dom::mixins::parent_node<T>::append(
        nodes_or_strings_t ...nodes)
        -> void
{
    // get the class that this mixin is being mixed into, and the node
    auto* base = static_cast<T*>(this);
    auto* node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);

    // append this node into this node's child nodes via mutation algorithms
    helpers::mutation_algorithms::append(node, this);

    // custom element reactions
    ce_reactions(&parent_node<T>::append);
}


template <typename T>
template <typename ...nodes_or_strings_t>
auto dom::mixins::parent_node<T>::replace_children(
        nodes_or_strings_t ...nodes)
        -> void
{
    // get the class that this mixin is being mixed into, and the node
    auto* base = static_cast<T*>(this);
    auto* node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);

    // ensure that pre-insertions are valid, and replace all with the node
    helpers::mutation_algorithms::ensure_pre_insertion_validity(node, this, nullptr);
    helpers::mutation_algorithms::replace_all(node, this);

    // custom element reactions
    ce_reactions(&parent_node<T>::replace_children);
}


template <typename T>
auto dom::mixins::parent_node<T>::get_children() const
        -> ext::vector<dom::nodes::element*>*
{
    // return all the children that are element type nodes
    return static_cast<const T*>(this)->child_nodes->template cast_all<nodes::element*>();
}


template <typename T>
auto dom::mixins::parent_node<T>::get_first_element_child() const
        -> dom::nodes::element*
{
    // return the first item from the element child list
    return children->front();
}


template <typename T>
auto dom::mixins::parent_node<T>::get_last_element_child() const
        -> dom::nodes::element*
{
    // return the last item from the element child list
    return children->back();
}


template <typename T>
auto dom::mixins::parent_node<T>::get_child_element_count() const
        -> size_t
{
    // return the length of the element child list
    return children->length();
}


template <typename T>
auto dom::mixins::parent_node<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<parent_node<dom::nodes::node>>{isolate}
            .inherit<dom_object>()
            .function("querySelector", &parent_node<dom::nodes::node>::query_selector)
            .function("querySelectorAll", &parent_node<dom::nodes::node>::query_selector_all)

            .var("children", &parent_node<dom::nodes::node>::children)
            .var("firstElementChild", &parent_node<dom::nodes::node>::first_element_child)
            .var("lastElementChild", &parent_node<dom::nodes::node>::last_element_child)
            .var("childElementCount", &parent_node<dom::nodes::node>::child_element_count)
            .auto_wrap_objects();
}


template class dom::mixins::parent_node<dom::nodes::element>;
template class dom::mixins::parent_node<dom::nodes::document>;
template class dom::mixins::parent_node<dom::nodes::document_fragment>;
