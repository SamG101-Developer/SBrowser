#pragma once
#ifndef SBROWSER_INFINITY_HPP
#define SBROWSER_INFINITY_HPP

#include <limits>

namespace {template <typename T> concept arithmetic = std::is_arithmetic_v<T>;}
namespace ext {template <arithmetic T> class infinity;}


template <arithmetic T>
struct ext::infinity {

public: operators
    operator T() const;

    auto operator -() const -> ext::infinity<T> requires (std::is_signed_v<T>);
    auto operator +() const -> ext::infinity<T> requires (std::is_signed_v<T>);

private: internal_properties
    bool m_positive = true;
};


template <arithmetic T>
ext::infinity<T>::operator T() const
{
    // convert the infinity type to the arithmetic T, taking the sign into account
    return std::numeric_limits<T>::infinity() * (m_positive ? 1 : -1);
}


template <arithmetic T>
auto ext::infinity<T>::operator -() const -> ext::infinity<T> requires (std::is_signed_v<T>)
{
    // applying the negative operator flips the sign
    return infinity<T>{.m_positive = not m_positive};
}


template <arithmetic T>
auto ext::infinity<T>::operator +() const -> ext::infinity<T> requires (std::is_signed_v<T>)
{
    // applying the positive operator makes the sign positive
    return infinity<T>{.m_positive = m_positive};
}


#endif //SBROWSER_INFINITY_HPP
