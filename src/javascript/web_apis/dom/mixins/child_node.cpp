#include "child_node.hpp"

#include <dom/nodes/character_data.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>

#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/trees.hpp>


template <typename T>
template <typename ...nodes_or_strings>
void
dom::mixins::child_node<T>::before(
        nodes_or_strings... nodes) {

    T* base = reinterpret_cast<T*>(this);

    if (nodes::node* parent = base->parent) {

        nodes::node* node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);
        nodes::node* viable_previous_sibling = helpers::trees::all_preceding_siblings(this)
                .filter([nodes = ext::vector{nodes...}](auto* node) {return not nodes.contains(node);})
                .front();

        viable_previous_sibling = not viable_previous_sibling
                ? parent->child_nodes->front()
                : viable_previous_sibling->next_sibling;

        helpers::mutation_algorithms::pre_insert(node, parent, viable_previous_sibling);
    }

    ce_reactions(&child_node<T>::before);
}


template <typename T>
template <typename ...nodes_or_strings>
void
dom::mixins::child_node<T>::after(
        nodes_or_strings... nodes) {

    T* base = reinterpret_cast<T*>(this);

    if (nodes::node* parent = base->parent) {

        nodes::node* node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);
        nodes::node* viable_next_sibling = helpers::trees::all_following_siblings(this)
                .filter([nodes = ext::vector{nodes...}](auto* node) {return not nodes.contains(node);})
                .front();

        helpers::mutation_algorithms::pre_insert(node, parent, viable_next_sibling);
    }

    ce_reactions(&child_node<T>::after);
}


template <typename T>
template <typename ...nodes_or_strings>
void
dom::mixins::child_node<T>::replace_with(
        nodes_or_strings ...nodes) {

    T* base = reinterpret_cast<T*>(this);

    if (nodes::node* parent = base->parent) {
        nodes::node* node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);
        nodes::node* viable_next_sibling = helpers::trees::all_following_siblings(this)
                .filter([nodes = ext::vector{nodes...}](auto* node) {return not nodes.contains(node);})
                .front();

        base->parent == parent
                ? helpers::mutation_algorithms::replace(this, node, parent)
                : helpers::mutation_algorithms::pre_insert(node, parent, viable_next_sibling);
    }

    ce_reactions(&child_node<T>::replace_with);
}


template <typename T>
void
dom::mixins::child_node<T>::remove() {

    T* base = reinterpret_cast<T*>(this);

    if (base->parent)
        helpers::mutation_algorithms::remove(base);

    ce_reactions(&child_node<T>::remove);
}


template class dom::mixins::child_node<dom::nodes::character_data>;
template class dom::mixins::child_node<dom::nodes::document_type>;
template class dom::mixins::child_node<dom::nodes::element>;
