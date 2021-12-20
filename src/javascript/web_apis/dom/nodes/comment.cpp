#include "comment.hpp"

#include <javascript/environment/realms.hpp>


dom::nodes::comment::comment(ext::cstring& new_data) : character_data() {

    data = new_data;
    owner_document = javascript::realms::current_global_object().get<document*>("associated_document");
}
