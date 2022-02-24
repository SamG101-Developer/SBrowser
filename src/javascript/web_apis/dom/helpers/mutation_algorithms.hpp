#pragma once
#ifndef SBROWSER_MUTATION_ALGORITHMS_HPP
#define SBROWSER_MUTATION_ALGORITHMS_HPP

namespace dom
{
    namespace helpers {struct mutation_algorithms;}
    namespace nodes {class node;}
}


struct dom::helpers::mutation_algorithms
{
    static auto common_checks(nodes::node* node, nodes::node* parent, nodes::node* child) -> void;
    static auto ensure_pre_insertion_validity(nodes::node* node, nodes::node* parent, nodes::node* child) -> void;

    static auto pre_insert(nodes::node* node, nodes::node* parent, nodes::node* child) -> nodes::node*;
    static auto pre_remove(nodes::node* node, nodes::node* parent) -> nodes::node*;

    static auto insert(nodes::node* node, nodes::node* parent, nodes::node* child, bool suppress_observers_flag = false) -> nodes::node*;
    static auto append(nodes::node* node, nodes::node* parent) -> nodes::node*;
    static auto replace(nodes::node* node, nodes::node* parent, nodes::node* child) -> nodes::node*;
    static auto remove(nodes::node* node, bool suppress_observers_flag = false) -> nodes::node*;

    static auto replace_all(nodes::node* node, nodes::node* parent) -> void;
};


#endif //SBROWSER_MUTATION_ALGORITHMS_HPP
