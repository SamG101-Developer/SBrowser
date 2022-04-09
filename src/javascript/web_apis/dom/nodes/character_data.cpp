#include "character_data.hpp"

#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>

#include <QtCore/QPointer>
#include <QtWidgets/QVBoxLayout>


dom::nodes::character_data::character_data()
        : node()
        , mixins::child_node<dom::nodes::character_data>()
        , mixins::non_document_type_child_node<dom::nodes::character_data>()
{
    // set the custom accessors
    length.getter = [this] {return get_length();};

    // create the widget representation
    auto widget = QPointer<QLabel>{};
    widget->setLayout(new QVBoxLayout{m_rendered_widget});
    widget->hide();
    widget->setWordWrap(true);
    m_rendered_widget = widget;
}


auto dom::nodes::character_data::substring_data(
        const ulong offset,
        const ulong count) const
        -> ext::string
{
    // return the substring_data helper method output
    return helpers::texts::substring_data(this, offset, count);
}


auto dom::nodes::character_data::append_data(
        const ext::string& new_data)
        -> void
{
    // return the replace_data helper method output by replacing 0 characters after length with the new_data
    return helpers::texts::replace_data(this, helpers::trees::length(this), 0, new_data);
}


auto dom::nodes::character_data::insert_data(
        const ulong offset,
        const ext::string& new_data)
        -> void
{
    // return the replace_data helper method output by replacing 0 characters after offset with new_data
    return helpers::texts::replace_data(this, offset, 0, new_data);
}


auto dom::nodes::character_data::delete_data(
        const ulong offset,
        const ulong count)
        -> void
{
    // return the delete_data helper method output by replacing count characters after offset with nothing
    return helpers::texts::replace_data(this, offset, count, "");
}


auto dom::nodes::character_data::replace_data(
        const ulong offset,
        const ulong count,
        const ext::string& new_data)
        -> void
{
    // return the replace_data helper method output by replacing count character after offset with new_data
    return helpers::texts::replace_data(this, offset, count, new_data);
}


auto dom::nodes::character_data::get_node_value() const
        -> ext::string
{
    // the node_value is the attribute's data
    return data;
}


auto dom::nodes::character_data::get_text_content() const
        -> ext::string
{
    // the text_content is the attribute's data
    return data;
}


auto dom::nodes::character_data::get_length() const
        -> ulong
{
    // the length is the length of the data
    return data->length();
}


auto dom::nodes::character_data::set_node_value(const ext::string& val)
        -> void
{
    // set the node_value by replacing the data with the val
    replace_data(0, length, val);
}


auto dom::nodes::character_data::set_text_content(const ext::string& val)
        -> void
{
    // set the text_content by replacing the data with the val
    replace_data(0, length, val);
}


auto dom::nodes::character_data::qt() const
        -> QLabel*
{
    return qobject_cast<QLabel*>(m_rendered_widget);
}


auto dom::nodes::character_data::v8(
        v8::Isolate* isolate) const
        -> ext::any
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

