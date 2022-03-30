#include "custom_vbox_layout.hpp"


auto render::custom_layouts::custom_vbox_layout::addItem(
        QLayoutItem* item)
        -> void
{
    emit widget_added(item->widget());
    QVBoxLayout::addItem(item);
}
