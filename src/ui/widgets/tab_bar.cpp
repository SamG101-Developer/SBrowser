#include "tab_bar.hpp"

#include <QtGui/QPainter>
#include <QtWidgets/QStyle>


ui::widgets::tab::tab(QWidget* parent) : QWidget(parent) {}


void ui::widgets::tab::setPixmap(QPixmap& pixmap) {
    m_pixmap = pixmap;
    update();
}


void ui::widgets::tab::paintEvent(QPaintEvent* event) {
    QPainter painter{this};
    painter.drawPixmap(0, 0, m_pixmap);
}


ui::widgets::tab_bar::tab_bar(QWidget* parent)
        : QTabBar(parent)
        , m_removed_tabs({})
        , m_animations({})
        , m_current_tab_width(0)
        , m_tab_clicked_index(-1)
        , m_last_hovered_tab(-1)
        , m_tab_border_radius(QStyle::PM_TabBarIconSize)
        , m_drag_object(nullptr)
        , m_drag_context(nullptr)
        , m_sliding_tab_object(nullptr)
        , m_sliding_tab_animations({})
        , m_sliding_tab_as_pixmap(QPixmap{})
        , m_sliding_tab_start_position(QPoint(0, 0))
        , m_slide_in_progress(false)

        , m_add_tab_button(new QPushButton{QIcon::fromTheme("add"), "", this}){

    setDrawBase(false);
    setAutoFillBackground(true);
    setElideMode(Qt::TextElideMode::ElideRight);
    setSelectionBehaviorOnRemove(QTabBar::SelectionBehavior::SelectPreviousTab);
    setContentsMargins(QMargins{m_tab_border_radius, 0, 0, 0});

    m_add_tab_button
}

