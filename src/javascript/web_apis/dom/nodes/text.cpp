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


dom::nodes::text*
dom::nodes::text::split_text(unsigned long offset)
{
    // return the split helper method output by splitting this text node at offset position
    return helpers::texts::split(this, offset);
}


QLabel*
dom::nodes::text::render()
{
    // cast the QWidget to a QLabel
    return qobject_cast<QLabel*>(m_rendered_widget);
}


INLINE ext::string
dom::nodes::text::get_whole_text() const
{
    // join the text from contiguous text nodes
    return helpers::trees::contiguous_text_nodes(this)
            .transform<ext::string>([](text* text_node) -> ext::string {return text_node->data;})
            .join(EMPTY);
}


INLINE void dom::nodes::text::set_data(ext::cstring& val)
{
    // set the data and update the label to show the new text
    data << val;
    render()->setText(data);
}
