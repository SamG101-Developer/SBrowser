#pragma once
#ifndef SBROWSER_LOGGING_HPP
#define SBROWSER_LOGGING_HPP

#include <ext/any.hpp>
#include <ext/string.hpp>

namespace console::logging {

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
}


#endif //SBROWSER_LOGGING_HPP
