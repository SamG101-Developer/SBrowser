#ifndef SBROWSER__BASE_WIDGET_HPP
#define SBROWSER__BASE_WIDGET_HPP

#include <ext/map.hpp>
#include <ext/string.hpp>

#include <QtGui/QColor>

namespace ui::widgets {class _base_widget;}


class ui::widgets::_base_widget {
    virtual void update_theme_slot(ext::cmap<ext::string, QColor>& theme) = 0;
};


#endif //SBROWSER__BASE_WIDGET_HPP
