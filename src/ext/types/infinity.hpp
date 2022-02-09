#pragma once
#ifndef SBROWSER_INFINITY_HPP
#define SBROWSER_INFINITY_HPP

#include <limits>

namespace {template <typename T> concept arithmetic = std::is_arithmetic_v<T>;}
namespace ext {template <arithmetic T> class infinity;}


template <arithmetic T>
struct ext::infinity {
    operator T() const;
    ext::infinity<T> operator -() const;
    ext::infinity<T> operator +() const;

private:
    bool m_positive = true;
};


template <arithmetic T>
ext::infinity<T>::operator T() const {
    return std::numeric_limits<T>::infinity() * (m_positive ? 1 : -1);
}

template <arithmetic T>
ext::infinity<T> ext::infinity<T>::operator -() const {
    return infinity<T>{.m_positive = not m_positive};
}

template <arithmetic T>
ext::infinity<T> ext::infinity<T>::operator +() const {
    return infinity<T>{.m_positive = m_positive};
}


#endif //SBROWSER_INFINITY_HPP
