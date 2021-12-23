#include "parent_node.hpp"

#include <dom/nodes/element.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>

#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/node_internals.hpp>


template<typename T>
dom::mixins::parent_node<T>::parent_node() {
    children.get = [this] {return get_children();};
    first_element_child.get = [this] {return get_first_element_child();};
    last_element_child.get = [this] {return get_last_element_child();};
    child_element_count.get = [this] {return get_child_element_count();};
}


template <typename T>
template <typename ...nodes_or_strings>
void
dom::mixins::parent_node<T>::prepend(
        nodes_or_strings ...nodes) {

    auto* base = reinterpret_cast<T*>(this);
    auto* node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);
    helpers::mutation_algorithms::pre_insert(node, this, base->children->front());

    ce_reactions(&parent_node<T>::prepend);
}


template <typename T>
template <typename ...nodes_or_strings>
void
dom::mixins::parent_node<T>::append(
        nodes_or_strings ...nodes) {

    auto* base = reinterpret_cast<T*>(this);
    auto* node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);
    helpers::mutation_algorithms::append(node, this);

    ce_reactions(&parent_node<T>::append);
}


template <typename T>
template <typename ...nodes_or_strings>
void
dom::mixins::parent_node<T>::replace_children(
        nodes_or_strings ...nodes) {

    auto* base = reinterpret_cast<T*>(this);
    auto* node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);
    helpers::mutation_algorithms::ensure_pre_insertion_validity(node, this, nullptr);
    helpers::mutation_algorithms::replace_all(node, this);

    ce_reactions(&parent_node<T>::replace_children);
}


template <typename T>
ext::vector<dom::nodes::element*>*
dom::mixins::parent_node<T>::get_children() const {
    return reinterpret_cast<const T*>(this)->child_nodes->cast_all<nodes::element*>();
}


template <typename T>
dom::nodes::element*
dom::mixins::parent_node<T>::get_first_element_child() const {
    return children->front();
}


template <typename T>
dom::nodes::element*
dom::mixins::parent_node<T>::get_last_element_child() const {
    return children->back();
}


template <typename T>
size_t
dom::mixins::parent_node<T>::get_child_element_count() const {
    return children->size();
}


template class dom::mixins::parent_node<dom::nodes::element>;
template class dom::mixins::parent_node<dom::nodes::document>;
template class dom::mixins::parent_node<dom::nodes::document_fragment>;
