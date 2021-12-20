#include "character_data.hpp"


dom::nodes::character_data::character_data()
        : node()
        , mixins::child_node<dom::nodes::character_data>()
        , mixins::non_document_type_child_node<dom::nodes::character_data>() {

    length.get = [this] {get_length();};
}


ext::string
dom::nodes::character_data::substring_data(
        unsigned long offset,
        unsigned long count) {

    return helpers::texts::substring_data(this, offset, count);
}


void
dom::nodes::character_data::append_data(
        ext::cstring& new_data) {

    return helpers::texts::replace_data(this, helpers::trees::length(this), 0, new_data);
}


void
dom::nodes::character_data::insert_data(
        unsigned long offset,
        ext::cstring& new_data) {

    return helpers::texts::replace_data(this, offset, 0, new_data);
}


void
dom::nodes::character_data::delete_data(
        unsigned long offset,
        unsigned long count) {

    return helpers::texts::replace_data(this, offset, count, "");
}


void
dom::nodes::character_data::replace_data(
        unsigned long offset,
        unsigned long count,
        ext::cstring& new_data) {

    return helpers::texts::replace_data(this, offset, count, new_data);
}


ext::string dom::nodes::character_data::get_node_value() const {return data;}
ext::string dom::nodes::character_data::get_text_content() const {return data;}
unsigned long dom::nodes::character_data::get_length() const {return data->length();}

void dom::nodes::character_data::set_node_value(ext::cstring& val) {replace_data(0, length, val);}
void dom::nodes::character_data::set_text_content(ext::cstring& val) {replace_data(0, length, val);}
