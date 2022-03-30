#include "trees.hpp"

#include <dom/nodes/attr.hpp>
#include <dom/nodes/cdata_section.hpp>
#include <dom/nodes/character_data.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/node.hpp>
#include <dom/nodes/text.hpp>


auto dom::helpers::trees::root(
        const nodes::node* const node)
        -> dom::nodes::node*
{
    // return nullptr if the node is nullptr
    if (not node)
        return nullptr;

    // return the topmost ancestor
    return ancestors(node).front();
}


auto dom::helpers::trees::descendants(
        const nodes::node* const node)
        -> ext::vector<dom::nodes::node*>
{
    // return an empty list if there are no children, otherwise return the flattened child nodes of the node; this
    // method runs recursively
    return node->child_nodes->empty()
            ? ext::vector<nodes::node*>{}
            : node->child_nodes->transform<ext::vector<nodes::node*>>([](const nodes::node* const child) {return descendants(child);}).flatten();
}


auto dom::helpers::trees::ancestors(
        const nodes::node* const node)
        -> ext::vector<dom::nodes::node*>
{
    // create the empty ancestors list and get the first parent (non-const node)
    ext::vector<nodes::node*> ancestors {};
    const auto* parent = node;

    // get the next parent if it exists
    while (parent->parent)
        ancestors.append(parent = node->parent);

    // return the ancestors
    return ancestors;
}


auto dom::helpers::trees::is_descendant(
        const nodes::node* const node_a,
        const nodes::node* const node_b)
        -> bool
{
    // return if node_b is contained in node_a's descendants (more efficient to flip nodes and check ancestors)
    return ancestors(node_a).contains(const_cast<nodes::node*>(node_b));
}


auto dom::helpers::trees::is_ancestor(
        const nodes::node* const node_a,
        const nodes::node* const node_b)
        -> bool
{
    // return if node_b is contained in node_a's ancestors
    return ancestors(node_b).contains(const_cast<nodes::node*>(node_a));
}


auto dom::helpers::trees::is_sibling(
        const nodes::node* const node_a,
        const nodes::node* const node_b)
        -> bool
{
    // return if the two parents are the same (and therefore must be siblings)
    return node_a->parent == node_b->parent;
}


auto dom::helpers::trees::is_preceding(
        const nodes::node* const node_a,
        const nodes::node* const node_b)
        -> bool
{
    // return if node_a comes before node_b by comparing node indexes
    return index(node_a) < index(node_b);
}


auto dom::helpers::trees::is_following(
        const nodes::node* const node_a,
        const nodes::node* const node_b)
        -> bool
{
    // return if node_a comes after node_b by comparing node indexes
    return index(node_a) > index(node_b);
}


auto dom::helpers::trees::index(
        const nodes::node* const node_a)
        -> unsigned long
{
    // return the index of the node in the descendants of the tree root
    return descendants(root(node_a)).find(const_cast<nodes::node*>(node_a));
}


auto dom::helpers::trees::length(
        const nodes::node* const node_a)
        -> unsigned long
{
    // return 0 if the node is an attribute or document type (no way to determine a length)
    if (multi_cast<const nodes::attr*, const nodes::document_type*>(node_a))
        return 0;

    // return the length of the data if the node is derived from character data (ie a textual node)
    else if(const auto* const character_data = dynamic_cast<const nodes::character_data*>(node_a))
        return character_data->data->length();

    // return the number of children in the node otherwise
    return node_a->child_nodes->length();
}


template <typename T>
auto dom::helpers::trees::all_following(
        const nodes::node* const node_a)
        -> ext::vector<dom::nodes::node*>
{
    // get the index of the node, and return all nodes that come after it, and can be cast to type T
    const auto index_a = index(node_a);
    return descendants(node_a)
            .template filter([index_a](nodes::node* descendant_node) {return index(descendant_node) > index_a;})
            .template cast_all<T>();
}


template <typename T>
auto dom::helpers::trees::all_preceding(
        const nodes::node* const node_a)
        -> ext::vector<dom::nodes::node*>
{
    // get the index of the node, and return all nodes that come before it, and can be cast to type T
    const auto index_a = index(node_a);
    return descendants(node_a)
            .template filter([index_a](nodes::node* descendant_node) {return index(descendant_node) < index_a;})
            .template cast_all<T>();
}


template <typename T>
auto dom::helpers::trees::all_following_siblings(
        const nodes::node* const node_a)
        -> ext::vector<dom::nodes::node*>
{
    // get the index of the node, and return all siblings that come after it, and can be cast to type T
    const auto index_a = index(node_a);
    return node_a->parent->child_nodes
            ->template filter([index_a](const nodes::node* const sibling_node) {return index(sibling_node) > index_a;})
            .template cast_all<T>();
}


template <typename T>
auto dom::helpers::trees::all_preceding_siblings(
        const nodes::node* const node_a)
        -> ext::vector<dom::nodes::node*>
{
    // get the index of the node, and return all siblings that come before it, and can be cast to type T
    const auto index_a = index(node_a);
    return node_a->parent->child_nodes
            ->template filter([index_a](const nodes::node* const sibling_node) {return index(sibling_node) < index_a;})
            .template cast_all<T>();
}


auto dom::helpers::trees::is_element_node(
        const nodes::node* const node_a)
        -> bool
{
    // syntactic sugar for checking if a node can be cast to an element pointer
    return dynamic_cast<const nodes::element*>(node_a) != nullptr;
}


auto dom::helpers::trees::is_text_node(
        const nodes::node* const node_a)
        -> bool
{
    // syntactic sugar for checking if a node can be cast to a text node pointer
    return dynamic_cast<const nodes::text*>(node_a) != nullptr;
}


auto dom::helpers::trees::is_document_type_node(
        const nodes::node* const node_a)
        -> bool
{
    // syntactic sugar for checking if a node can be cast to a document type pointer
    return dynamic_cast<const nodes::document_type*>(node_a) != nullptr;
}


auto dom::helpers::trees::descendant_text_content(
        const nodes::node* const node_a)
        -> ext::string
{
    // return empty string for nullptr node
    if (not node_a) return "";

    // get the descendant nodes that are text nodes, and join the data stored in them
    auto text_content = descendants(node_a)
            .template cast_all<nodes::text*>()
            .template transform<ext::string>([](nodes::text* descendant_text_node) -> ext::string {return descendant_text_node->data;})
            .join();

    // wrap and return the text content
    return ext::string{text_content};
}


auto dom::helpers::trees::child_text_content(
        const nodes::node* const node_a)
        -> ext::string
{
    // return empty string for nullptr node
    if (not node_a) return "";

    // get the child nodes that are text nodes, and join the data stored in them
    auto text_content = node_a->child_nodes
            ->template cast_all<nodes::text*>()
            .template transform<ext::string>([](nodes::text* child_text_node) -> ext::string {return child_text_node->data;})
            .join();

    // wrap and return the text content
    return ext::string{text_content};
}


auto dom::helpers::trees::descendant_text_nodes(
        const nodes::node* const node_a)
        -> ext::vector<dom::nodes::text*>
{
    return descendants(node_a).template cast_all<nodes::text*>();
}


auto dom::helpers::trees::contiguous_text_nodes(
        const nodes::node* const node_a)
        -> ext::vector<dom::nodes::text*>
{

    ext::vector<nodes::node*> siblings = node_a->parent->child_nodes;

    return ext::vector<nodes::text*>{}
            .extend(siblings.cast_all<nodes::text*>().slice(siblings.find(node_a), siblings.length()))
            .extend(siblings.cast_all<nodes::text*>().slice(0, siblings.find(node_a)).reversed(), 0);
}


auto dom::helpers::trees::is_exclusive_text_node(
        const nodes::node* const node_a)
        -> bool
{
    // return if the node is a text node and not a cdatasection node (ie not inherited from)
    return not dynamic_cast<const nodes::cdata_section*>(node_a) and dynamic_cast<const nodes::text*>(node_a);
}


auto dom::helpers::trees::common_ancestor(
        const nodes::node* const node_a,
        const nodes::node* const node_b)
        -> dom::nodes::node*
{
    auto node_a_ancestors = ancestors(node_a);
    auto node_b_ancestors = ancestors(node_b);
    return node_a_ancestors.intersection(node_b_ancestors).back();
}



