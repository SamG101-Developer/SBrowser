#include "document_type.hpp"


dom::nodes::document_type::document_type()
        : dom::nodes::node() {

    public_id = "";
    system_id = "";
    node_name = name;
}
