#pragma once
#ifndef SBROWSER_LOGGING_HPP
#define SBROWSER_LOGGING_HPP

#include <stack>

#include <ext/iterables/string.hpp>
#include <ext/types/any.hpp>

namespace console::logging {
    static std::stack<ext::string> group_stack;

    template <typename ...data_t> void assert_(bool condition, data_t&&... data);
    template <typename ...data_t> void debug(data_t&&... data);
    template <typename ...data_t> void error(data_t&&... data);
    template <typename ...data_t> void info(data_t&&... data);
    template <typename ...data_t> void log(data_t&&... data);
    template <typename ...data_t> void warn(data_t&&... data);

    template <typename ...data_t> void trace(data_t&&... data);
    template <typename ...data_t> void dir_xml(data_t&&... data);

    void table(const ext::any& tabular_data, const ext::vector<ext::string>& properties_list = {});
    void dir(const ext::any& item = 0, const ext::any& options = {});

    void clear();
}


#endif //SBROWSER_LOGGING_HPP
