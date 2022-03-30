#pragma once
#ifndef SBROWSER_CUSTOM_VBOX_LAYOUT_HPP
#define SBROWSER_CUSTOM_VBOX_LAYOUT_HPP

#include <ext/decorators.hpp>

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLayoutItem>

namespace render::custom_layouts {class custom_vbox_layout;}


class render::custom_layouts::custom_vbox_layout : public QVBoxLayout
{
signals:
    void widget_added(QWidget*);

public methods:
    auto addItem(QLayoutItem* item) -> void override;
};


#endif //SBROWSER_CUSTOM_VBOX_LAYOUT_HPP
