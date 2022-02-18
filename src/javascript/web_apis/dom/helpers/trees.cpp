#include "trees.hpp"

#include <dom/nodes/attr.hpp>
#include <dom/nodes/cdata_section.hpp>
#include <dom/nodes/character_data.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/node.hpp>
#include <dom/nodes/text.hpp>


dom::nodes::node*
dom::helpers::trees::root(const nodes::node* node)
{
    // return nullptr if the node is nullptr
    if (not node)
        return nullptr;

    // return the topmost ancestor
    return ancestors(node).front();
}


ext::vector<dom::nodes::node*>
dom::helpers::trees::descendants(const nodes::node* node)
{
    // return an empty list if there are no children, otherwise return the flattened child nodes of the node; this
    // method runs recursively
    return node->child_nodes->empty()
            ? ext::vector<nodes::node*>{}
            : node->child_nodes->transform<>([](nodes::node* child) {return descendants(child);}).flatten();
}


ext::vector<dom::nodes::node*>
dom::helpers::trees::ancestors(const nodes::node* node)
{
    // create the empty ancestors list and get the first parent (non-const node)
    ext::vector<nodes::node*> ancestors {};
    auto* parent = const_cast<nodes::node*>(node);

    // get the next parent if it exists
    while (parent->parent)
        ancestors.append(parent = node->parent);

    // return the ancestors
    return ancestors;
}


bool
dom::helpers::trees::is_descendant(const nodes::node* node_a, const nodes::node* node_b)
{
    // return if node_b is contained in node_a's descendants (more efficient to flip nodes and check ancestors)
    return ancestors(node_a).contains(const_cast<nodes::node*>(node_b));
}


bool
dom::helpers::trees::is_ancestor(const nodes::node* node_a, const nodes::node* node_b)
{
    // return if node_b is contained in node_a's ancestors
    return ancestors(node_b).contains(const_cast<nodes::node*>(node_a));
}


bool
dom::helpers::trees::is_sibling(const nodes::node* node_a, const nodes::node* node_b)
{
    // return if the two parents are the same (and therefore must be siblings)
    return node_a->parent == node_b->parent;
}


bool
dom::helpers::trees::is_preceding(const nodes::node* node_a, const nodes::node* node_b)
{
    // return if node_a comes before node_b by comparing node indexes
    return index(node_a) < index(node_b);
}


bool
dom::helpers::trees::is_following(const nodes::node* node_a, const nodes::node* node_b)
{
    // return if node_a comes after node_b by comparing node indexes
    return index(node_a) > index(node_b);
}


unsigned long
dom::helpers::trees::index(const nodes::node* node_a)
{
    // return the index of the node in the descendants of the tree root
    return descendants(root(node_a)).find(const_cast<nodes::node*>(node_a));
}


unsigned long
dom::helpers::trees::length(const nodes::node* node_a)
{
    // return 0 if the node is an attribute or document type (no way to determine a length)
    if (multi_cast<const nodes::attr*, const nodes::document_type*>(node_a))
        return 0;

    // return the length of the data if the node is derived from character data (ie a textual node)
    else if(auto* character_data = dynamic_cast<const nodes::character_data*>(node_a))
        return character_data->data->length();

    // return the number of children in the node otherwise
    return node_a->child_nodes->length();
}


template <typename T>
ext::vector<dom::nodes::node*>
dom::helpers::trees::all_following(const nodes::node* node_a)
{
    // get the index of the node, and return all nodes that come after it, and can be cast to type T
    const auto index_a = index(node_a);
    return descendants(node_a)
            .template filter([index_a](nodes::node* descendant_node) {return index(descendant_node) > index_a;})
            .template cast_all<T>();
}


template <typename T>
ext::vector<dom::nodes::node*>
dom::helpers::trees::all_preceding(const nodes::node* node_a)
{
    // get the index of the node, and return all nodes that come before it, and can be cast to type T
    const auto index_a = index(node_a);
    return descendants(node_a)
            .template filter([index_a](nodes::node* descendant_node) {return index(descendant_node) < index_a;})
            .template cast_all<T>();
}


template <typename T>
ext::vector<dom::nodes::node*>
dom::helpers::trees::all_following_siblings(const nodes::node* node_a)
{
    // get the index of the node, and return all siblings that come after it, and can be cast to type T
    const auto index_a = index(node_a);
    return node_a->parent->child_nodes
            ->template filter([index_a](nodes::node* sibling_node) {return index(sibling_node) > index_a;})
            .template cast_all<T>();
}


template <typename T>
ext::vector<dom::nodes::node*>
dom::helpers::trees::all_preceding_siblings(const nodes::node* node_a)
{
    // get the index of the node, and return all siblings that come before it, and can be cast to type T
    const auto index_a = index(node_a);
    return node_a->parent->child_nodes
            ->template filter([index_a](nodes::node* sibling_node) {return index(sibling_node) < index_a;})
            .template cast_all<T>();
}


bool dom::helpers::trees::is_element_node(const nodes::node* node_a)
{
    // syntactic sugar for checking if a node can be cast to an element pointer
    return dynamic_cast<const nodes::element*>(node_a) != nullptr;
}


bool dom::helpers::trees::is_text_node(const nodes::node* node_a)
{
    // syntactic sugar for checking if a node can be cast to a text node pointer
    return dynamic_cast<const nodes::text*>(node_a) != nullptr;
}


bool dom::helpers::trees::is_document_type_node(const nodes::node* node_a)
{
    // syntactic sugar for checking if a node can be cast to a document type pointer
    return dynamic_cast<const nodes::document_type*>(node_a) != nullptr;
}


ext::string
dom::helpers::trees::descendant_text_content(const nodes::node* node_a)
{
    // get the descendant nodes that are text nodes, and join the data stored in them
    auto text_content = descendants(node_a)
            .template cast_all<nodes::text*>()
            .template transform<ext::string>([](nodes::text* descendant_text_node) -> ext::string {return descendant_text_node->data;})
            .join();

    // wrap and return the text content
    return ext::string{text_content};
}


ext::string
dom::helpers::trees::child_text_content(const nodes::node* node_a)
{
    // get the child nodes that are text nodes, and join the data stored in them
    auto text_content = node_a->child_nodes
            ->template cast_all<nodes::text*>()
            .template transform<ext::string>([](auto* child_text_node) -> ext::string {return child_text_node->data;})
            .join();

    // wrap and return the text content
    return ext::string{text_content};
}


ext::vector<dom::nodes::text*>
dom::helpers::trees::descendant_text_nodes(const nodes::node* node_a)
{
    return descendants(node_a).template cast_all<nodes::text*>();
}


ext::vector<dom::nodes::text*>
dom::helpers::trees::contiguous_text_nodes(const nodes::node* node_a)
{

    ext::vector<nodes::node*> siblings = node_a->parent->child_nodes;

    return ext::vector<nodes::text*>{}
            .extend(siblings.slice(siblings.find(const_cast<nodes::node*>(node_a)), siblings.length()).cast_all<nodes::text*>())
            .extend(siblings.slice(0, siblings.find(const_cast<nodes::node*>(node_a))).cast_all<nodes::text*>().reversed(), 0);
}


bool
dom::helpers::trees::is_exclusive_text_node(const nodes::node* node_a)
{

    return not dynamic_cast<const nodes::cdata_section*>(node_a) and dynamic_cast<const nodes::text*>(node_a);
}


dom::nodes::node*
dom::helpers::trees::common_ancestor(
        const nodes::node* node_a,
        const nodes::node* node_b)
{
    auto node_a_ancestors = ancestors(node_a);
    auto node_b_ancestors = ancestors(node_b);
    return node_a_ancestors.intersection(node_b_ancestors).back();
}



