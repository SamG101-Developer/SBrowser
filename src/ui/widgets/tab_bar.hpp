#pragma once
#ifndef SBROWSER_TAB_BAR_HPP
#define SBROWSER_TAB_BAR_HPP

#include <ext/any.hpp>
#include <ext/vector.hpp>

#include <QtCore/QVariantAnimation>

#include <QtWidgets/QTabBar>
#include <QtWidgets/QPushButton>

#include <QtGui/QDrag>


namespace ui {
    namespace widgets {
        class tab_bar;
        class tab;
    }

    namespace internal {
        class drag;
    }
}


class ui::widgets::tab : public QWidget {
public:
    explicit tab(QWidget* parent = nullptr);

public:
    void setPixmap(QPixmap& pixmap) override;
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap m_pixmap;
};


class ui::widgets::tab_bar : public QTabBar {
public:
    explicit tab_bar(QWidget* parent);

protected:
    ext::vector<ext::string_any_map> m_removed_tabs;
    ext::vector<QVariantAnimation*> m_animations;
    short m_current_tab_width;
    short m_tab_clicked_index;
    short m_last_hovered_tab;
    short m_tab_border_radius;

    QDrag* m_drag_object;
    internal::drag* m_drag_context;

    QWidget* m_sliding_tab_object;
    ext::vector<QVariantAnimation*> m_sliding_tab_animations;
    QPixmap m_sliding_tab_as_pixmap;
    QPoint m_sliding_tab_start_position;
    bool m_slide_in_progress;

protected:
    QPushButton* m_add_tab_button;

protected signals:

};


#endif //SBROWSER_TAB_BAR_HPP
