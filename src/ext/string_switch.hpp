#pragma once
#ifndef SBROWSER_STRING_SWITCH_HPP
#define SBROWSER_STRING_SWITCH_HPP


namespace {

constexpr unsigned long a = 54059;
constexpr unsigned long b = 76963;
constexpr unsigned long c = 86969;
constexpr unsigned long d = 00037;

constexpr std::size_t hashext::cstring&(const char* string) {
    auto h = d;
    while (*string) {
        h = (h * a) ^ (string[0] * b);
        string++;
    }

    return h;
}

}


#define string_switch(string) switch(hashext::cstring&(string))
#define string_case(string)   case  (hashext::cstring&(string))
#define string_default        default


#endif //SBROWSER_STRING_SWITCH_HPP
