#pragma once
#ifndef SBROWSER_TRAVERSAL_HPP
#define SBROWSER_TRAVERSAL_HPP

namespace dom {
    namespace helpers {struct traversal;}
    namespace iterators {
        class abstract_iterator;
        class node_iterator;
        class tree_walker;}
    namespace nodes {class node;}
}

enum traversal_direction {NEXT, PREVIOUS};
enum traversal_child {FIRST_CHILD, LAST_CHILD};
enum traversal_sibling {NEXT_SIBLING, PREVIOUS_SIBLING};


struct dom::helpers::traversal {
    static unsigned short filter(nodes::node* node, iterators::abstract_iterator* iterator);
    static nodes::node* traverse(iterators::node_iterator* iterator, traversal_direction direction);
    static nodes::node* traverse_children(iterators::tree_walker* iterator, traversal_child type);
    static nodes::node* traverse_siblings(iterators::tree_walker* iterator, traversal_sibling type);
    static nodes::node* traverse_node_previous(iterators::tree_walker* iterator);
    static nodes::node* traverse_node_next(iterators::tree_walker* iterator);
};


#endif //SBROWSER_TRAVERSAL_HPP
