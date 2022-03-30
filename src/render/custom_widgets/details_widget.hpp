#pragma once
#ifndef SBROWSER_DETAILS_WIDGET_HPP
#define SBROWSER_DETAILS_WIDGET_HPP

#include <ext/decorators.hpp>
#include <QtCore/QPointer>
#include <QtWidgets/QWidget>

namespace render::custom_widgets {class details_widget;}


class render::custom_widgets::details_widget : public QWidget
{
public constructors:
    details_widget(QWidget* parent = nullptr);

public js_methods:
    void mouseReleaseEvent(QMouseEvent* event) override;

protected cpp_properties:
    QPointer<QWidget> m_details{};
};


#endif //SBROWSER_DETAILS_WIDGET_HPP
