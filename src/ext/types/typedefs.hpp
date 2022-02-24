#ifndef SBROWSER_TYPEDEFS_HPP
#define SBROWSER_TYPEDEFS_HPP

#include <ext/iterables/string.hpp>
#include <ext/types/any.hpp>

#define struct_string inline static ext::cstring


auto operator==(ext::cany& first, ext::cany& second) -> bool
{
    // equality check by comparing the addresses of the two objects
    return &first == &second;
}


auto operator==(const std::function<void()>& function_a, const std::function<void()>& function_b) -> bool
{
    // equality check by comparing the addresses of the two objects
    return &function_a == &function_b;
}

#endif //SBROWSER_TYPEDEFS_HPP
