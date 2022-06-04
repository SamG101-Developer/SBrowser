#include "comment.hpp"

#include <javascript/environment/realms.hpp>


dom::nodes::comment::comment(const ext::string& new_data)
{
    // set custom properties
    node_type      = COMMENT_NODE;
    data           = new_data;
    owner_document = &javascript::realms::current_realm(this).get<document&>("associated_document");
}


auto dom::nodes::comment::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<comment>{isolate}
            .ctor<>()
            .ctor<const ext::string&>()
            .inherit<character_data>()
            .auto_wrap_objects();
}
