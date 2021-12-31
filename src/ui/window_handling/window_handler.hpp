#ifndef SBROWSER_WINDOW_HANDLER_HPP
#define SBROWSER_WINDOW_HANDLER_HPP

#include <ext/iterables/vector.hpp>

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>

#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

#include <QtGui/QFont>
#include <QtGui/QFontDatabase>
#include <QtGui/QIcon>

namespace ui::window_handling {
    class window_handler;
    class window_context;
    class browser_window;
}


struct ui::window_handling::window_context {
    long theme;
    long language;
    long search_engine;
    long border_radius;
    QFont current_font;
};


class ui::window_handling::window_handler : public QObject {
public:
    explicit window_handler(QObject* parent);

public:
    bool create_window();
    bool add_window(browser_window* window);
    bool remove_window(browser_window* window);

private:
    ext::vector<browser_window*> m_windows;
    window_context m_context;
};

#endif //SBROWSER_WINDOW_HANDLER_HPP
