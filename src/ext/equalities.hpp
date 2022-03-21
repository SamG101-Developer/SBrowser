#ifndef SBROWSER_EQUALITIES_HPP
#define SBROWSER_EQUALITIES_HPP

#include <ext/string.hpp>
#include <ext/any.hpp>


auto operator==(const ext::any& first, const ext::any& second) -> bool
{
    // equality check by comparing the addresses of the two objects
    return &first == &second;
}


auto operator==(const std::function<void()>& function_a, const std::function<void()>& function_b) -> bool
{
    // equality check by comparing the addresses of the two objects
    return &function_a == &function_b;
}

#endif //SBROWSER_EQUALITIES_HPP
