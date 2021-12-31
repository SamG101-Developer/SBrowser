#pragma once
#ifndef SBROWSER_COUNTING_HPP
#define SBROWSER_COUNTING_HPP

#include <ext/iterables/map.hpp>

namespace console::counting {
    static ext::map<ext::string, size_t> count_map {};

    void count(ext::cstring& label);
    void count_reset(ext::cstring& label);
}


#endif //SBROWSER_COUNTING_HPP
