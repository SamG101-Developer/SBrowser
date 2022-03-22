#include "comment.hpp"

#include <javascript/environment/realms.hpp>


/*
 * https://dom.spec.whatwg.org/#dom-comment-comment
 * https://developer.mozilla.org/en-US/docs/Web/API/Comment/Comment
 *
 * The Comment() constructor returns a newly created Comment object with the optional string given in parameter as its
 * textual content.
 */
dom::nodes::comment::comment(const ext::string& new_data) : character_data()
{
    // set custom properties
    node_type << COMMENT_NODE;
    data << new_data;
    owner_document << &javascript::realms::current_global_object().get<document&>("associated_document");
}


auto dom::nodes::comment::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<comment>{isolate}
            .template ctor<>()
            .template ctor<const ext::string&>()
            .template inherit<character_data>()
            .auto_wrap_objects();
}
