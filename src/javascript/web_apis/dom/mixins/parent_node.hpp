#pragma once
#ifndef SBROWSER_PARENT_NODE_HPP
#define SBROWSER_PARENT_NODE_HPP

#include <ext/types/property.hpp>
#include <ext/iterables/vector.hpp>

namespace dom {
    namespace mixins {template <typename T> class parent_node;}
    namespace nodes {
        class element;
        class node;
    }
}


template <typename T>
class dom::mixins::parent_node {
public:
    parent_node();

public:
    template<typename ...nodes_or_strings> void prepend(nodes_or_strings... nodes);
    template<typename ...nodes_or_strings> void append(nodes_or_strings... nodes);
    template<typename ...nodes_or_strings> void replace_children(nodes_or_strings... nodes);

    nodes::element* query_selector(ext::string selectors);
    ext::vector<nodes::node*> query_selector_all(ext::string selectors);

public:
    ext::dom_property<ext::vector<nodes::element*>*> children;
    ext::dom_property<nodes::element*> first_element_child;
    ext::dom_property<nodes::element*> last_element_child;
    ext::dom_property<size_t> child_element_count;

private:
    ext::vector<nodes::element*>* get_children() const;
    nodes::element* get_first_element_child() const;
    nodes::element* get_last_element_child() const;
    size_t get_child_element_count() const;
};


#endif //SBROWSER_PARENT_NODE_HPP
