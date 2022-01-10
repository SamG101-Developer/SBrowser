#include "trees.hpp"

#include <dom/nodes/attr.hpp>
#include <dom/nodes/cdata_section.hpp>
#include <dom/nodes/character_data.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/node.hpp>
#include <dom/nodes/text.hpp>


dom::nodes::node*
dom::helpers::trees::root(const nodes::node* node) {

    if (not node) return nullptr;

    while (node->parent) node = node->parent;
    return node;
}


ext::vector<dom::nodes::node*>
dom::helpers::trees::descendants(const nodes::node* node) {

    return node->child_nodes->empty()
            ? ext::vector<nodes::node*>{}
            : node->child_nodes->transform<>([](auto* child) {return descendants(child);}).flatten();
}


ext::vector<dom::nodes::node*>
dom::helpers::trees::ancestors(const nodes::node* node) {

    ext::vector<nodes::node*> ancestors {};
    auto* parent = const_cast<nodes::node*>(node);

    while (parent->parent)
        ancestors.append(parent = node->parent);

    return ancestors;
}


bool dom::helpers::trees::is_descendant(const nodes::node* node_a, const nodes::node* node_b) {
    return ancestors(node_a).contains(node_b);
}


bool dom::helpers::trees::is_ancestor(const nodes::node* node_a, const nodes::node* node_b) {
    return ancestors(node_b).contains(node_a);
}


bool dom::helpers::trees::is_sibling(const nodes::node* node_a, const nodes::node* node_b) {
    return node_a->parent == node_b->parent;
}


bool dom::helpers::trees::is_preceding(const nodes::node* node_a, const nodes::node* node_b) {
    return index(node_a) < index(node_b);
}


bool dom::helpers::trees::is_following(const nodes::node* node_a, const nodes::node* node_b) {
    return index(node_a) > index(node_b);
}


unsigned long dom::helpers::trees::index(const nodes::node* node_a) {
    return descendants(root(node_a)).find(node_a);
}


unsigned long dom::helpers::trees::length(const nodes::node* node_a) {
    if (dynamic_cast<const nodes::attr*>(node_a) or dynamic_cast<const nodes::document_type*>(node_a))
        return 0;
    else if(auto* character_data = dynamic_cast<const nodes::character_data*>(node_a))
        return character_data->data->length();
    return node_a->child_nodes->length();
}


template <typename T>
ext::vector<dom::nodes::node*>
dom::helpers::trees::all_following(const nodes::node* node_a) {

    const auto index_a = index(node_a);
    return descendants(node_a)
            .template filter([index_a](auto* descendant_node) {return index(descendant_node) > index_a;})
            .template cast_all<T>();
}


template <typename T>
ext::vector<dom::nodes::node*>
dom::helpers::trees::all_preceding(const nodes::node* node_a) {

    const auto index_a = index(node_a);
    return descendants(node_a)
            .template filter([index_a](auto* descendant_node) {return index(descendant_node) < index_a;})
            .template cast_all<T>();
}


template <typename T>
ext::vector<dom::nodes::node*>
dom::helpers::trees::all_following_siblings(const nodes::node* node_a) {

    const auto index_a = index(node_a);
    return node_a->parent->child_nodes
            ->template filter([index_a](auto* sibling_node) {return index(sibling_node) > index_a;})
            .template cast_all<T>();
}


template <typename T>
ext::vector<dom::nodes::node*>
dom::helpers::trees::all_preceding_siblings(const nodes::node* node_a) {

    const auto index_a = index(node_a);
    return node_a->parent->child_nodes
            ->template filter([index_a](auto* sibling_node) {return index(sibling_node) < index_a;})
            .template cast_all<T>();
}


bool dom::helpers::trees::is_element_node(const nodes::node* node_a) {
    return dynamic_cast<const nodes::element*>(node_a) != nullptr;
}


bool dom::helpers::trees::is_text_node(const nodes::node* node_a) {
    return dynamic_cast<const nodes::text*>(node_a) != nullptr;
}


bool dom::helpers::trees::is_document_type_node(const nodes::node* node_a) {
    return dynamic_cast<const nodes::document_type*>(node_a) != nullptr;
}


ext::string
dom::helpers::trees::descendant_text_content(const nodes::node* node_a) {

    return descendants(node_a)
            .template cast_all<nodes::text*>()
            .template transform<ext::string>([](auto* descendant_text_node) -> ext::string {return descendant_text_node->data;})
            .join();
}


ext::string
dom::helpers::trees::child_text_content(const nodes::node* node_a) {

    return node_a->child_nodes
            ->template cast_all<nodes::text*>()
            .template transform<ext::string>([](auto* child_text_node) -> ext::string {return child_text_node->data;})
            .join();
}


ext::vector<dom::nodes::text*>
dom::helpers::trees::descendant_text_nodes(const nodes::node* node_a) {
    return descendants(node_a).template cast_all<nodes::text*>();
}


ext::vector<dom::nodes::text*>
dom::helpers::trees::contiguous_text_nodes(const nodes::node* node_a) {

    ext::vector<nodes::node*> siblings = node_a->parent->child_nodes;

    return ext::vector<nodes::text*>{}
            .extend(siblings.slice(siblings.find(node_a), siblings.length()).cast_all<nodes::text*>())
            .extend(siblings.slice(0, siblings.find(node_a)).cast_all<nodes::text*>().reversed(), 0);
}


bool
dom::helpers::trees::is_exclusive_text_node(const nodes::node* node_a) {

    return not dynamic_cast<nodes::cdata_section*>(node_a) and dynamic_cast<nodes::text*>(node_a);
}


dom::nodes::node*
dom::helpers::trees::common_ancestor(
        const nodes::node* node_a,
        const nodes::node* node_b) {

    auto node_a_ancestors = ancestors(node_a);
    auto node_b_ancestors = ancestors(node_b);
    return node_a_ancestors.intersection(node_b_ancestors).back();
}



