#include "child_node.hpp"

#include <dom/nodes/character_data.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>

#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/trees.hpp>


template <typename T>
template <typename ...nodes_or_strings_t>
auto dom::mixins::child_node<T>::before(nodes_or_strings_t... nodes) -> void
{
    // get the class that this mixin is being mixed into
    auto* base = static_cast<T*>(this);

    // check that the node has a parent (otherwise has no siblings)
    if (nodes::node* const parent = base->parent)
    {
        // get the first viable previous sibling (previous sibling closest to the node, that isn't contained by nodes)
        nodes::node* const node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);
        nodes::node* viable_previous_sibling =
                helpers::trees::all_preceding_siblings(this)
                .template last_match([nodes = ext::vector{nodes...}](nodes::node* node) {return not nodes.contains(node);});

        // if there is no viable previous sibling then use the first sibling, otherwise the sibling after the viable
        viable_previous_sibling = not viable_previous_sibling
                ? parent->child_nodes->front().value()
                : viable_previous_sibling->next_sibling;

        // pre-insert the node before the viable next sibling
        helpers::mutation_algorithms::pre_insert(node, parent, viable_previous_sibling);
    }

    // custom element reactions
    handle_ce_reactions(&child_node<T>::before);
}


template <typename T>
template <typename ...nodes_or_strings_t>
auto dom::mixins::child_node<T>::after(nodes_or_strings_t... nodes) -> void
{
    // get the class that this mixin is being mixed into
    auto* base = static_cast<T*>(this);

    // check that the node has a parent (otherwise has no siblings)
    if (nodes::node* const parent = base->parent)
    {
        // get the first viable next sibling (next sibling closest to the node, that isn't contained by nodes)
        nodes::node* const node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);
        nodes::node* const viable_next_sibling =
                helpers::trees::all_following_siblings(this)
                .template first_match([nodes = ext::vector{nodes...}](nodes::node* node) {return not nodes.contains(node);});

        // pre-insert the node before the viable previous sibling
        helpers::mutation_algorithms::pre_insert(node, parent, viable_next_sibling);
    }

    // custom element reactions
    ce_reactions(&child_node<T>::after);
}


template <typename T>
template <typename ...nodes_or_strings_t>
auto dom::mixins::child_node<T>::replace_with(nodes_or_strings_t ...nodes) -> void
{
    // get the class that this mixin is being mixed into
    auto* base = static_cast<T*>(this);

    // check that the node has a parent (otherwise has no siblings)
    if (nodes::node* parent = base->parent)
    {
        // get the first viable next sibling (next sibling closest to the node, that isn't contained by nodes)
        nodes::node* node = helpers::node_internals::convert_nodes_into_node(base->owner_document, nodes...);
        nodes::node* const viable_next_sibling =
                helpers::trees::all_following_siblings(this)
                .template first_match([nodes = ext::vector{nodes...}](nodes::node* node) {return not nodes.contains(node);});

        base->parent == parent
                ? helpers::mutation_algorithms::replace(this, node, parent)
                : helpers::mutation_algorithms::pre_insert(node, parent, viable_next_sibling);
    }

    // custom element reactions
    ce_reactions(&child_node<T>::replace_with);
}


template <typename T>
auto dom::mixins::child_node<T>::remove() -> void
{
    // get the class that this mixin is being mixed into
    auto* base = static_cast<T*>(this);

    // remove this node if it has a parent
    if (base->parent)
        helpers::mutation_algorithms::remove(base);

    // custom element reactions
    ce_reactions(&child_node<T>::remove);
}


template <typename T>
auto dom::mixins::child_node<T>::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<dom::mixins::child_node<dom::nodes::node>>{isolate}
            .inherit<dom_object>()
            .auto_wrap_objects();
}


template class dom::mixins::child_node<dom::nodes::character_data>;
template class dom::mixins::child_node<dom::nodes::document_type>;
template class dom::mixins::child_node<dom::nodes::element>;
