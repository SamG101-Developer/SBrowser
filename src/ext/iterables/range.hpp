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
    func inclusive_contains(T value) const -> bool;
    func exclusive_contains(T value) const -> bool;

private: internal_properties
    const T m_low;
    const T m_high;
};


template <typename T>
ext::range<T>::range(T low, T high) requires std::is_arithmetic_v<T> : m_low(low) , m_high(high)
{

}


template <typename T>
func ext::range<T>::inclusive_contains(T value) const -> bool
{
    // check that a number is inbetween the bounds, including the bounds
    return (m_low <= value) and (value <= m_high);
}


template <typename T>
func ext::range<T>::exclusive_contains(T value) const -> bool
{
    // check that a number is inbetween the bounds, excluding the bounds
    return (m_low < value) and (value < m_high);
}


#endif //SBROWSER_RANGE_HPP
