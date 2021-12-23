#pragma once
#ifndef SBROWSER_CHILD_NODE_HPP
#define SBROWSER_CHILD_NODE_HPP

#include <javascript/interop/attribute_descriptors.hpp>

namespace dom::mixins {template <typename T> class child_node;}


template <typename T>
class dom::mixins::child_node {
    unscopable template<typename ...nodes_or_strings> void before(nodes_or_strings... nodes);
    unscopable template<typename ...nodes_or_strings> void after(nodes_or_strings... nodes);
    unscopable template<typename ...nodes_or_strings> void replace_with(nodes_or_strings... nodes);
    unscopable void remove();
};


#endif //SBROWSER_CHILD_NODE_HPP
