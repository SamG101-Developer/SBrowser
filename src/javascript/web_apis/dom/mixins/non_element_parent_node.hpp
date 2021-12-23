#pragma once
#ifndef SBROWSER_NON_ELEMENT_PARENT_NODE_HPP
#define SBROWSER_NON_ELEMENT_PARENT_NODE_HPP

#include <ext/string.hpp>

namespace dom {
    namespace mixins {template <typename T> class non_element_parent_node;}
    namespace nodes {class element;}
}


template <typename T>
class dom::mixins::non_element_parent_node {
public:
    nodes::element* get_element_by_id(ext::cstring& element_id);
};


#endif //SBROWSER_NON_ELEMENT_PARENT_NODE_HPP
