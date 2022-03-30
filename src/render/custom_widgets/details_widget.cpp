#include "details_widget.hpp"

#include <render/custom_layouts/custom_vbox_layout.hpp>


render::custom_widgets::details_widget::details_widget(
        QWidget* parent)

        : QWidget(parent)
{
    m_details->setLayout(new QVBoxLayout{});

    auto* layout = new custom_layouts::custom_vbox_layout{};
    connect(layout, SIGNAL(custom_layouts::custom_vbox_layout::widget_added(QLayoutItem*)), m_details->layout(), SLOT(QVBoxLayout::addItem(QLayoutItem*)));
    setLayout(layout);
}
