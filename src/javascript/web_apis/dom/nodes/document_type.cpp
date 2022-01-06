#include "document_type.hpp"


dom::nodes::document_type::document_type()
        : node()
        , mixins::child_node<document_type>() {

    node_type = DOCUMENT_TYPE_NODE;
    public_id = "";
    system_id = "";
    node_name = name;
}
