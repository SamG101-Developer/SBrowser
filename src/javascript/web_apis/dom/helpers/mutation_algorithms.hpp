#pragma once
#ifndef SBROWSER_MUTATION_ALGORITHMS_HPP
#define SBROWSER_MUTATION_ALGORITHMS_HPP

namespace dom {
    namespace helpers {struct mutation_algorithms;}
    namespace nodes {class node;}
}


struct dom::helpers::mutation_algorithms {
    static void common_checks(nodes::node* node, nodes::node* parent, nodes::node* child);

    static void ensure_pre_insertion_validity(nodes::node* node, nodes::node* parent, nodes::node* child);
    static nodes::node* pre_insert(nodes::node* node, nodes::node* parent, nodes::node* child);
    static nodes::node* pre_remove(nodes::node* node, nodes::node* parent);

    static nodes::node* insert(nodes::node* node, nodes::node* parent, nodes::node* child, bool suppress_observers_flag = false);
    static nodes::node* append(nodes::node* node, nodes::node* parent);
    static nodes::node* replace(nodes::node* node, nodes::node* parent, nodes::node* child);
    static nodes::node* remove(nodes::node* node, bool suppress_observers_flag = false);

    static void replace_all(nodes::node* node, nodes::node* parent);
};


#endif //SBROWSER_MUTATION_ALGORITHMS_HPP
