#include "text.hpp"

#include <dom/helpers/texts.hpp>
#include <dom/helpers/trees.hpp>

#include <QtWidgets/QVBoxLayout>


dom::nodes::text::text(ext::cstring& new_data)
        : character_data()
        , mixins::slottable<text>()
{
    // set the custom accessors
    whole_text.get = [this] {return get_whole_text();};

    data.set = [this](auto&& PH1) {set_data(std::forward<decltype(PH1)>(PH1));};

    // set the properties
    node_type << TEXT_NODE;
    data << new_data;

    // create the widget representation
    m_rendered_widget = new QLabel{};
    render()->setLayout(new QVBoxLayout{m_rendered_widget});
    render()->hide();
    render()->setWordWrap(true);
}


auto dom::nodes::text::split_text(unsigned long offset) -> text*
{
    // return the split helper method output by splitting this text node at offset position
    return helpers::texts::split(this, offset);
}


auto dom::nodes::text::render() const -> QLabel*
{
    // cast the QWidget to a QLabel
    return qobject_cast<QLabel*>(m_rendered_widget);
}


auto dom::nodes::text::get_whole_text() const -> ext::string
{
    // join the text from contiguous text nodes
    return helpers::trees::contiguous_text_nodes(this)
            .transform<ext::string>([](text* text_node) -> ext::string {return text_node->data;})
            .join(EMPTY_CHAR);
}


auto dom::nodes::text::set_data(ext::cstring& val) -> void
{
    // set the data and update the label to show the new text
    data << val;
    render()->setText(data);
}


auto dom::nodes::text::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<text>{isolate}
            .ctor<>()
            .ctor<ext::cstring&>()
            .inherit<character_data>()
            .inherit<mixins::slottable<text>>()
            .function("splitText", &text::split_text)
            .var("wholeText", &text::whole_text, true)
            .auto_wrap_objects();
}
