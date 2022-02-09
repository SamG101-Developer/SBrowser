#ifndef SBROWSER_LABEL_HPP
#define SBROWSER_LABEL_HPP

#include <ui/widgets/_base_widget.hpp>
#include <QtWidgets/QLabel>

namespace ui::widgets {class label;}


class ui::widgets::label
        : public QLabel
        , public _base_widget {

public:
    label(QWidget* parent, bool inverted);

    void update_theme_slot(ext::cmap<ext::string, QColor>& theme) override;

private:
    bool m_inverted;
};


#endif //SBROWSER_LABEL_HPP
