#pragma once
#ifndef SBROWSER_INFINITY_HPP
#define SBROWSER_INFINITY_HPP

#include <limits>

namespace {template <typename T> concept arithmetic = std::is_arithmetic_v<T>;}
namespace ext {template <arithmetic T> class infinity;}


template <arithmetic T>
struct ext::infinity {

    operator T() const {return std::numeric_limits<T>::infinity() * (m_positive ? 1 : -1);}
    ext::infinity<T> operator -() const {return infinity<T>{.m_positive = not m_positive};}
    ext::infinity<T> operator +() const {return infinity<T>{.m_positive = m_positive};}

private:
    bool m_positive = true;
};


#endif //SBROWSER_INFINITY_HPP
