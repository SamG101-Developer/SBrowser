#pragma once
#ifndef SBROWSER_INFINITY_HPP
#define SBROWSER_INFINITY_HPP

#include <limits>

namespace ext {template <typename T, typename = std::is_arithmetic<T>> class infinity;}


template <typename T, typename>
struct ext::infinity {

    operator T() const {return std::numeric_limits<T>::infinity() * (m_positive ? 1 : -1);}
    ext::infinity<T> operator -() const {return infinity<T>{.m_positive = not m_positive};}
    ext::infinity<T> operator +() const {return infinity<T>{.m_positive = true};}

private:
    bool m_positive = true;
};


#endif //SBROWSER_INFINITY_HPP
