#pragma once
#ifndef SBROWSER_INFINITY_HPP
#define SBROWSER_INFINITY_HPP

#include <limits>

namespace {template <typename T> concept Arithmetic = std::is_arithmetic_v<T>;}
namespace ext {template <Arithmetic T> class infinity;}


template <Arithmetic T>
struct ext::infinity final
{
public operators:
    operator T() const;

    auto operator -() const -> ext::infinity<T> requires (std::is_signed_v<T>);
    auto operator +() const -> ext::infinity<T> requires (std::is_signed_v<T>);

    template <typename ...Args> static auto is_inf(Args&&... value) -> bool;
    template <typename ...Args> static auto is_nan(Args&&... value) -> bool;
    template <typename ...Args> static auto is_inf_or_nan(Args&&... value) -> bool;

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
auto ext::infinity<T>::operator -() const -> ext::infinity<T> requires (std::is_signed_v<T>)
{
    // applying the negative operator flips the sign
    return infinity<T>{.m_positive = not m_positive};
}


template <Arithmetic T>
auto ext::infinity<T>::operator +() const -> ext::infinity<T> requires (std::is_signed_v<T>)
{
    // applying the positive operator makes the sign positive
    return infinity<T>{.m_positive = m_positive};
}


template <Arithmetic T>
template <typename ...Args>
auto ext::infinity<T>::is_inf(Args&&... value) -> bool
{
    return (std::isinf(std::forward<Args>(value)), ...);
}


template <Arithmetic T>
template <typename ...Args>
auto ext::infinity<T>::is_nan(Args&&... value) -> bool
{
    return (std::isnan(std::forward<Args>(value)), ...);
}


template <Arithmetic T>
template <typename ...Args>
auto ext::infinity<T>::is_inf_or_nan(Args&&... value) -> bool
{
    return ((is_inf(value) or is_nan(value)), ...); // TODO -> is this all_of or any_of? needs to be any_of
}


#endif //SBROWSER_INFINITY_HPP
