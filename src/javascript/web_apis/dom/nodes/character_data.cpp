#include "character_data.hpp"

#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>


dom::nodes::character_data::character_data()
        : node()
        , mixins::child_node<dom::nodes::character_data>()
        , mixins::non_document_type_child_node<dom::nodes::character_data>()
{
    // set the custom accessors
    length.get = [this] {return get_length();};
}


ext::string
dom::nodes::character_data::substring_data(
        unsigned long offset,
        unsigned long count) const
{
    // return the substring_data helper method output
    return helpers::texts::substring_data(this, offset, count);
}


void
dom::nodes::character_data::append_data(
        ext::cstring& new_data)
{
    // return the replace_data helper method output by replacing 0 characters after length with the new_data
    return helpers::texts::replace_data(this, helpers::trees::length(this), 0, new_data);
}


void
dom::nodes::character_data::insert_data(
        unsigned long offset,
        ext::cstring& new_data)
{
    // return the replace_data helper method output by replacing 0 characters after offset with new_data
    return helpers::texts::replace_data(this, offset, 0, new_data);
}


void
dom::nodes::character_data::delete_data(
        unsigned long offset,
        unsigned long count)
{
    // return the delete_data helper method output by replacing count characters after offset with nothing
    return helpers::texts::replace_data(this, offset, count, "");
}


void
dom::nodes::character_data::replace_data(
        unsigned long offset,
        unsigned long count,
        ext::cstring& new_data)
{
    // return the replace_data helper method output by replacing count character after offset with new_data
    return helpers::texts::replace_data(this, offset, count, new_data);
}


INLINE ext::string
dom::nodes::character_data::get_node_value() const
{
    // the node_value is the attribute's data
    return data;
}


INLINE ext::string
dom::nodes::character_data::get_text_content() const
{
    // the text_content is the attribute's data
    return data;
}


INLINE unsigned long
dom::nodes::character_data::get_length() const
{
    // the length is the length of the data
    return data->length();
}


INLINE void
dom::nodes::character_data::set_node_value(ext::cstring& val)
{
    // set the node_value by replacing the data with the val
    replace_data(0, length, val);
}


INLINE void
dom::nodes::character_data::set_text_content(ext::cstring& val)
{
    // set the text_content by replacing the data with the val
    replace_data(0, length, val);
}
