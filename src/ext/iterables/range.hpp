#ifndef SBROWSER_RANGE_HPP
#define SBROWSER_RANGE_HPP

#include <deque>
#include <ext/iterables/iterable.hpp>

namespace ext {template <typename T> class range;}


template <typename T>
class ext::range : public ext::iterable<size_t, std::deque<T>>{
public: constructors
    range(T low, T high) requires std::is_arithmetic_v<T>;

public: methods
    // algorithms
    bool inclusive_contains(T value) const;
    bool exclusive_contains(T value) const;

private: internal_properties
    const T m_low;
    const T m_high;
};


template <typename T>
ext::range<T>::range(T low, T high) requires std::is_arithmetic_v<T>
        : m_low(low)
        , m_high(high) {
}

template <typename T>
bool ext::range<T>::inclusive_contains(T value) const {
    return (m_low <= value) and (value <= m_high);
}

template <typename T>
bool ext::range<T>::exclusive_contains(T value) const {
    return (m_low < value) and (value < m_high);
}


#endif //SBROWSER_RANGE_HPP
