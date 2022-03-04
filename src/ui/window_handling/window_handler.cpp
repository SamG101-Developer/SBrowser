#include "window_handler.hpp"

#include <fstream>
#include <sstream>
#include <string>

#include <ext/iterables/string.hpp>


ui::window_handling::window_handler::window_handler(
        QObject* parent):
        m_windows({}) {

    std::ofstream context_file_contents;
    context_file_contents.open("../resources/", std::ofstream::out);

    ext::stringstream context_file_stream;
    context_file_contents << context_file_contents.rdbuf();

    ext::string context_file_string;
    context_file_string = context_file_stream.str();

    auto attributes = context_file_string.split(',');
    auto font_attribute = attributes.at(4).split(',');
    auto font_name = QString(font_attribute.at(0));
    auto font_size = (short){font_attribute.at(1)};

    m_context = {
        .theme = attributes.at(0),
        .language = attributes.at(1),
        .search_engine = attributes.at(2),
        .border_radius = attributes.at(3),
        .current_font = QFont(font_name, font_size)
    };
}


bool ui::window_handling::window_handler::add_window(browser_window* window) {
    m_windows.append(window);
}


bool ui::window_handling::window_handler::remove_window(browser_window* window) {
    m_windows.remove(window);
}


bool ui::window_handling::window_handler::create_window() {
    m_windows.append(new browser_window{this});
}
