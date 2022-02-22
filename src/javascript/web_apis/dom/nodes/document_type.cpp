#include "document_type.hpp"


dom::nodes::document_type::document_type()
        : node()
        , mixins::child_node<document_type>() {

    node_type << DOCUMENT_TYPE_NODE;
    public_id << "";
    system_id << "";
    node_name << name;
}


auto dom::nodes::document_type::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<dom::nodes::document_type>{isolate}
            .inherit<dom::nodes::node>()
            .inherit<dom::mixins::child_node<dom::nodes::document_type>>()
            .var("name", &dom::nodes::document_type::name)
            .var("publicId", &dom::nodes::document_type::public_id)
            .var("systemId", &dom::nodes::document_type::system_id)
            .auto_wrap_objects();
}
