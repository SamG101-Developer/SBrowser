#pragma once
#ifndef SBROWSER_STRING_SWITCH_HPP
#define SBROWSER_STRING_SWITCH_HPP

#include <ext/string.hpp>


namespace
{
    const ulong a = 54059;
    const ulong b = 76963;
    const ulong c = 86969;
    const ulong d = 00037;

    constexpr auto hash(const char* s) -> size_t
    {
        // create the hash object h
        auto h = d;

        // loop until the end of the string is reached
        while (*s)
        {
            // update the hash and increment the string
            h = (h * a) ^ (s[0] * b);
            s++;
        }

        // return the hash object
        return h;
    }
}


#define string_switch(string) switch(::hash(string))
#define string_case(string) case(::hash(string))
#define string_default default


#endif //SBROWSER_STRING_SWITCH_HPP
