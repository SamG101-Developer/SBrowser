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
            .template inherit<node>()
            .template inherit<mixins::child_node<document_type>>()
            .template var("name", &document_type::name)
            .template var("publicId", &document_type::public_id)
            .template var("systemId", &document_type::system_id)
            .auto_wrap_objects();
}
