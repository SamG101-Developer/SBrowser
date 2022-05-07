#pragma once
#ifndef SBROWSER_INFINITY_HPP
#define SBROWSER_INFINITY_HPP

#include <limits>

template <typename T> concept Arithmetic = std::is_arithmetic_v<T>;
namespace ext {template <Arithmetic T> class infinity;}


template <Arithmetic T>
struct ext::infinity final
{
public constructors:
    infinity() = default;

public operators:
    operator T() const;

    auto operator -() const -> infinity;
    auto operator +() const -> infinity;
    auto operator -() const -> infinity requires (std::is_signed_v<T>);
    auto operator +() const -> infinity requires (std::is_signed_v<T>);

private cpp_properties:
    bool m_positive = true;
};


template <Arithmetic T>
ext::infinity<T>::operator T() const
{
    // convert the infinity type to the arithmetic T, taking the sign into account
    return std::numeric_limits<T>::infinity() * (m_positive ? 1 : -1);
}


template <Arithmetic T>
auto ext::infinity<T>::operator-() const -> infinity
{
    // default behaviour is to do nothing on a negative conversion (no guarantee that T is signed)
    return *this;
}


template <Arithmetic T>
auto ext::infinity<T>::operator+() const -> infinity
{
    // default behaviour is to do nothing on a positive conversion (no guarantee that T is signed)
    return *this;
}


template <Arithmetic T>
auto ext::infinity<T>::operator-() const -> infinity requires (std::is_signed_v<T>)
{
    // applying the negative operator flips the sign
    return infinity<T>{.m_positive = not m_positive};
}


template <Arithmetic T>
auto ext::infinity<T>::operator+() const -> infinity requires (std::is_signed_v<T>)
{
    // applying the positive operator makes the sign positive
    return infinity<T>{.m_positive = m_positive};
}


#endif //SBROWSER_INFINITY_HPP
