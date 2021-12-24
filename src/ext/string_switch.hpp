#pragma once
#ifndef SBROWSER_STRING_SWITCH_HPP
#define SBROWSER_STRING_SWITCH_HPP

#include <ext/string.hpp>


namespace {

constexpr unsigned long a = 54059;
constexpr unsigned long b = 76963;
constexpr unsigned long c = 86969;
constexpr unsigned long d = 00037;

constexpr size_t hash(const char* s) {
    auto h = d;
    while (*s) {
        h = (h * a) ^ (s[0] * b);
        s++;
    }

    return h;
}
}


#define string_switch(string) switch(hash(string))
#define string_case(string) case(hash(string))
#define string_default default


#endif //SBROWSER_STRING_SWITCH_HPP
