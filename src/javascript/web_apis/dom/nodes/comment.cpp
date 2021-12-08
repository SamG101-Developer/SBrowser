#include "comment.hpp"

#include <v8pp/convert.hpp>


dom::nodes::comment::comment(ext::cstring& new_data)
        : dom::nodes::character_data() {

    data = new_data;
    owner_document = v8pp::convert<document*>::from_v8(javascript::realms::current_global_object()->Global().Get(
            javascript::realms::current_global_object(),
            "associated_document"));
}
