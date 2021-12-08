#include "text.hpp"

#include <QtWidgets/QVBoxLayout>


dom::nodes::text::text(ext::cstring& new_data)
        : dom::nodes::character_data()
        , dom::mixins::slottable<dom::nodes::character_data>() {

    whole_text.get = [this] {get_whole_text();};
    data.set = [this](auto&& PH1) {set_data(std::forward<decltype(PH1)>(PH1));};

    m_rendered_widget = new QLabel{};
    render()->setLayout(new QVBoxLayout{m_rendered_widget});
    render()->hide();
    render()->setWordWrap(true);

    data = new_data;
}


dom::nodes::text*
dom::nodes::text::split_text(unsigned long offset) {
    return helpers::texts::split_text(this, offset);
}


QLabel*
dom::nodes::text::render() {
    return qobject_cast<QLabel*>(m_rendered_widget);
}


ext::string dom::nodes::text::get_whole_text() const {
    return helpers::trees::contiguous_exclusive_text_nodes(this)
            .transform<ext::string>([](text* text_node) -> ext::string {return text_node->data;})
            .join("");
}


void dom::nodes::text::set_data(ext::cstring& val) {
    data << val;
    render()->setText(data);
}
