#ifndef SBROWSER_TYPEDEFS_HPP
#define SBROWSER_TYPEDEFS_HPP

#include <ext/string.hpp>

#define struct_string static constexpr const char*

inline bool operator==(const std::function<void()>& function_a, const std::function<void()>& function_b) {
    return &function_a == &function_b;
}

#endif //SBROWSER_TYPEDEFS_HPP
