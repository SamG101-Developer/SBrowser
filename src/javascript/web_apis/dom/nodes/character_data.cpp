#include "character_data.hpp"

#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>


dom::nodes::character_data::character_data()
        : node()
        , mixins::child_node<dom::nodes::character_data>()
        , mixins::non_document_type_child_node<dom::nodes::character_data>()
{
    // set the custom accessors
    length.getter = [this] {return get_length();};
}


/*
 * https://dom.spec.whatwg.org/#dom-characterdata-substringdata
 * https://developer.mozilla.org/en-US/docs/Web/API/CharacterData/substringData
 *
 * The substringData() method of the CharacterData interface returns a portion of the existing data, starting at the
 * specified index and extending for a given number of characters afterwards.
 */
auto dom::nodes::character_data::substring_data(
        unsigned long offset,
        unsigned long count) const
        -> ext::string
{
    // return the substring_data helper method output
    return helpers::texts::substring_data(this, offset, count);
}


/*
 * https://dom.spec.whatwg.org/#dom-characterdata-appenddata
 * https://developer.mozilla.org/en-US/docs/Web/API/CharacterData/appendData
 *
 * The appendData() method of the CharacterData interface adds the provided data to the end of the node's current data.
 */
auto dom::nodes::character_data::append_data(
        const ext::string& new_data)
        -> void
{
    // return the replace_data helper method output by replacing 0 characters after length with the new_data
    return helpers::texts::replace_data(this, helpers::trees::length(this), 0, new_data);
}


/*
 * https://dom.spec.whatwg.org/#dom-characterdata-insertdata
 * https://developer.mozilla.org/en-US/docs/Web/API/CharacterData/insertData
 *
 * The insertData() method of the CharacterData interface inserts the provided data into this CharacterData node's
 * current data, at the provided offset from the start of the existing data. The provided data is spliced into the
 * existing data.
 */
auto dom::nodes::character_data::insert_data(
        unsigned long offset,
        const ext::string& new_data)
        -> void
{
    // return the replace_data helper method output by replacing 0 characters after offset with new_data
    return helpers::texts::replace_data(this, offset, 0, new_data);
}


/*
 * https://dom.spec.whatwg.org/#dom-characterdata-deletedata
 * https://developer.mozilla.org/en-US/docs/Web/API/CharacterData/deleteData
 *
 * The deleteData() method of the CharacterData interface removes all or part of the data from this CharacterData node.
 */
auto dom::nodes::character_data::delete_data(
        unsigned long offset,
        unsigned long count)
        -> void
{
    // return the delete_data helper method output by replacing count characters after offset with nothing
    return helpers::texts::replace_data(this, offset, count, "");
}


/*
 * https://dom.spec.whatwg.org/#dom-characterdata-replacedata
 * https://developer.mozilla.org/en-US/docs/Web/API/CharacterData/replaceData
 *
 * The replaceData() method of the CharacterData interface removes a certain number of characters of the existing text
 * in a given CharacterData node and replaces those characters with the text provided.
 * in a given CharacterData node and replaces those characters with the text provided.
 */
auto dom::nodes::character_data::replace_data(
        unsigned long offset,
        unsigned long count,
        const ext::string& new_data)
        -> void
{
    // return the replace_data helper method output by replacing count character after offset with new_data
    return helpers::texts::replace_data(this, offset, count, new_data);
}


auto dom::nodes::character_data::get_node_value() const -> ext::string
{
    // the node_value is the attribute's data
    return data;
}


auto dom::nodes::character_data::get_text_content() const -> ext::string
{
    // the text_content is the attribute's data
    return data;
}


auto dom::nodes::character_data::get_length() const -> unsigned long
{
    // the length is the length of the data
    return data->length();
}


auto dom::nodes::character_data::set_node_value(const ext::string& val) -> void
{
    // set the node_value by replacing the data with the val
    replace_data(0, length, val);
}


auto dom::nodes::character_data::set_text_content(const ext::string& val) -> void
{
    // set the text_content by replacing the data with the val
    replace_data(0, length, val);
}


auto dom::nodes::character_data::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<character_data>{isolate}
            .inherit<node>()
            .inherit<dom::mixins::child_node<character_data>>()
            .inherit<dom::mixins::non_document_type_child_node<character_data>>()

            .function("substringData", &character_data::substring_data)
            .function("appendData", &character_data::append_data)
            .function("insertData", &character_data::insert_data)
            .function("replaceData", &character_data::replace_data)
            .function("deleteData", &character_data::delete_data)
            .var("data", &character_data::data)
            .var("length", &character_data::length, true)
            .auto_wrap_objects();
}

