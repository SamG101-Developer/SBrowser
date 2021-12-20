#include "ranges.hpp"

#include <dom/helpers/exceptions.hpp>

#include <dom/nodes/node.hpp>


dom::nodes::node*
dom::helpers::ranges::check_parent_exists(nodes::node* node) {

    helpers::exceptions::throw_v8_exception(
            "node must have a parent",
            INVALID_NODE_TYPE_ERR,
            [node] -> bool {return not node->parent_node;});

    return node->parent_node;
}
