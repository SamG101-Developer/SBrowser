#ifndef SBROWSER_RANGE_HPP
#define SBROWSER_RANGE_HPP

#include <xtr1common>

namespace ext {template <typename T> class range;}


template <typename T>
class ext::range {
public:
    range(T low, T high) requires std::is_arithmetic_v<T>
            : m_low(low)
            , m_high(high) {
    };

    bool inclusive_contains(T value) {
        return (m_low <= value) and (value <= m_high);
    }

    bool exclusive_contains(T value) {
        return (m_low < value) and (value < m_high);
    }

private:
    const T m_low;
    const T m_high;
};


#endif //SBROWSER_RANGE_HPP
