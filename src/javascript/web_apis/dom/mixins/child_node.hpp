#pragma once
#ifndef SBROWSER_CHILD_NODE_HPP
#define SBROWSER_CHILD_NODE_HPP

#include <ext/macros/decorators.hpp>
#include <javascript/interop/attribute_descriptors.hpp>

namespace dom::mixins {template <typename T> class child_node;}


template <typename T>
class dom::mixins::child_node {
public: methods

    unscopable template<typename ...nodes_or_strings_t> void before(nodes_or_strings_t... nodes);
    unscopable template<typename ...nodes_or_strings_t> void after(nodes_or_strings_t... nodes);
    unscopable template<typename ...nodes_or_strings_t> void replace_with(nodes_or_strings_t... nodes);
    unscopable void remove();
};


#endif //SBROWSER_CHILD_NODE_HPP
