#include "document_type.hpp"


dom::nodes::document_type::document_type()
        : node()
        , mixins::child_node<document_type>() {

    // set the properties
    node_type << DOCUMENT_TYPE_NODE;
    public_id << "";
    system_id << "";
    node_name << name;
}


auto dom::nodes::document_type::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<document_type>{isolate}
            .inherit<node>()
            .inherit<mixins::child_node<document_type>>()
            .var("name", &document_type::name)
            .var("publicId", &document_type::public_id)
            .var("systemId", &document_type::system_id)
            .auto_wrap_objects();
}
