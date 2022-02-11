#pragma once
#ifndef SBROWSER_PARENT_NODE_HPP
#define SBROWSER_PARENT_NODE_HPP

#include <ext/iterables/vector.hpp>
#include <ext/properties/dom_property.hpp>

namespace dom {
    namespace mixins {template <typename T> class parent_node;}
    namespace nodes {
        class element;
        class node;
    }
}


template <typename T>
class dom::mixins::parent_node {
public: constructors
    parent_node();

public: methods
    template<typename ...nodes_or_strings_t> void prepend(nodes_or_strings_t... nodes);
    template<typename ...nodes_or_strings_t> void append(nodes_or_strings_t... nodes);
    template<typename ...nodes_or_strings_t> void replace_children(nodes_or_strings_t... nodes);

    nodes::element* query_selector(ext::string selectors);
    ext::vector<nodes::node*> query_selector_all(ext::string selectors);

public: properties
    ext::dom_property<ext::vector<nodes::element*>*, _F> children;
    ext::dom_property<nodes::element*, _F> first_element_child;
    ext::dom_property<nodes::element*, _F> last_element_child;
    ext::dom_property<size_t, _F> child_element_count;

private: accessors
    ext::vector<nodes::element*>* get_children() const;
    nodes::element* get_first_element_child() const;
    nodes::element* get_last_element_child() const;
    size_t get_child_element_count() const;
};


#endif //SBROWSER_PARENT_NODE_HPP
