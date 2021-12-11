#include "trees.hpp"

#include <dom/nodes/attr.hpp>
#include <dom/nodes/character_data.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/node.hpp>


dom::nodes::node*
dom::helpers::trees::root(nodes::node* node) {

    if (not node) return nullptr;

    while (node->parent_node) node = node->parent_node;
    return node;
}


ext::vector<dom::nodes::node*>
dom::helpers::trees::descendants(nodes::node* node) {

    return node->child_nodes->empty()
            ? {}
            : node->child_nodes->transform<>([](auto* child) {return descendants(child);}).flatten();
}


ext::vector<dom::nodes::node*>
dom::helpers::trees::ancestors(nodes::node* node) {

    return node->parent_node
            ? node->parent_node
            : node;
}


bool dom::helpers::trees::is_descendant(nodes::node* node_a, nodes::node* node_b) {
    return ancestors(node_a).contains(node_b);
}


bool dom::helpers::trees::is_ancestor(nodes::node* node_a, nodes::node* node_b) {
    return ancestors(node_b).contains(node_a);
}


bool dom::helpers::trees::is_sibling(nodes::node* node_a, nodes::node* node_b) {
    return node_a->parent_node == node_b->parent_node;
}


bool dom::helpers::trees::is_preceding(nodes::node* node_a, nodes::node* node_b) {
    return index(node_a) < index(node_b);
}


bool dom::helpers::trees::is_following(nodes::node* node_a, nodes::node* node_b) {
    return index(node_a) > index(node_b);
}


unsigned long dom::helpers::trees::index(nodes::node* node_a) {
    return descendants(root(node_a)).find(node_a);
}


unsigned long dom::helpers::trees::length(nodes::node* node_a) {
    if (dynamic_cast<nodes::attr*>(node_a) or dynamic_cast<nodes::document_type*>(node_a))
        return 0;
    else if(auto* character_data = dynamic_cast<nodes::character_data*>(node_a))
        return character_data->data->length();
    return node_a->child_nodes->length();
}
