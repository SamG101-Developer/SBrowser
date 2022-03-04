#pragma once
#ifndef SBROWSER_TRAVERSAL_HPP
#define SBROWSER_TRAVERSAL_HPP

namespace dom
{
    namespace helpers {struct traversal;}
    namespace iterators
    {
        class abstract_iterator;
        class node_iterator;
        class tree_walker;
    }
    namespace nodes {class node;}
}

enum class traversal_direction {NEXT, PREVIOUS};
enum class traversal_child {FIRST_CHILD, LAST_CHILD};
enum class traversal_sibling {NEXT_SIBLING, PREVIOUS_SIBLING};


struct dom::helpers::traversal
{
    static auto filter(const nodes::node* node, iterators::abstract_iterator* iterator) -> unsigned short;
    static auto traverse(iterators::node_iterator* iterator, traversal_direction direction) -> nodes::node*;
    static auto traverse_children(iterators::tree_walker* iterator, traversal_child type) -> nodes::node*;
    static auto traverse_siblings(iterators::tree_walker* iterator, traversal_sibling type) -> nodes::node*;
    static auto traverse_node_previous(iterators::tree_walker* iterator) -> nodes::node*;
    static auto traverse_node_next(iterators::tree_walker* iterator) -> nodes::node*;
};


#endif //SBROWSER_TRAVERSAL_HPP
