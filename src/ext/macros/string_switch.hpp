#pragma once
#ifndef SBROWSER_STRING_SWITCH_HPP
#define SBROWSER_STRING_SWITCH_HPP

#include <ext/iterables/string.hpp>


namespace {

const unsigned long a = 54059;
const unsigned long b = 76963;
const unsigned long c = 86969;
const unsigned long d = 00037;

size_t hash(const char* s) {
    auto h = d;
    while (*s) {
        h = (h * a) ^ (s[0] * b);
        s++;
    }

    return h;
}
}


#define string_switch(string) switch(hash(string.c_str()))
#define string_case(string) case(hash(string.c_str()))
#define string_default default


#endif //SBROWSER_STRING_SWITCH_HPP
