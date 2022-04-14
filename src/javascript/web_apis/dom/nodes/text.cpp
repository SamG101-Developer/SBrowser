#include "text.hpp"

#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>


dom::nodes::text::text(const ext::string& new_data)
        : character_data()
        , mixins::slottable<text>()
{
    // set the custom accessor methods
    whole_text.getter = [this] {return get_whole_text();};

    data.setter = [this](auto&& PH1) {set_data(std::forward<decltype(PH1)>(PH1));};

    // set the property values
    data      << new_data;
    node_type << TEXT_NODE;
}


auto dom::nodes::text::split_text(
        ulong offset)
        -> text*
{
    // return the split helper method output by splitting this text node at offset position
    return helpers::texts::split(this, offset);
}


auto dom::nodes::text::get_whole_text() const
        -> ext::string
{
    // join the text from contiguous text nodes
    return helpers::trees::contiguous_text_nodes(this)
            .transform<ext::string>([](text* text_node) -> ext::string {return text_node->data;})
            .join(EMPTY_CHAR);
}


auto dom::nodes::text::set_data(
        const ext::string& val)
        -> void
{
    // set the data and update the label to show the new text
    data << val;
    qobject_cast<QLabel*>(m_rendered_widget)->setText(data);
}


auto dom::nodes::text::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<text>{isolate}
            .ctor<>()
            .ctor<const ext::string&>()
            .inherit<character_data>()
            .inherit<mixins::slottable<text>>()
            .function("splitText", &text::split_text)
            .var("wholeText", &text::whole_text, true)
            .auto_wrap_objects();
}
