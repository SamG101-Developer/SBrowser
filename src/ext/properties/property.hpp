#ifndef SBROWSER_PROPERTY_HPP
#define SBROWSER_PROPERTY_HPP

#include <functional>
#include <type_traits>

#include <ext/macros/cpp_keywords.hpp>
#include <ext/macros/decorators.hpp>
#include <ext/types/any.hpp>

namespace ext
{
    template <typename T> struct property;

    template <typename U, typename T> auto property_dynamic_cast(const ext::property<T>& o) -> U;
    template <typename U, typename T> auto property_static_cast(const ext::property<T>& o) -> U;
    template <typename U, typename T> auto property_const_cast(const ext::property<T>& o) -> U;
    template <typename U, typename T> auto property_reinterpret_cast(const ext::property<T>& o) -> U;
    template <typename U, typename T> auto property_any_cast(const ext::property<T>& o) -> U;
    template <typename U, typename T> auto property_bit_cast(const ext::property<T>& o) -> U;
}


template <typename T>
struct ext::property
{
public: friends
    // override get
    friend T operator>>(const property<T>& p, T& o) {o = p.m_internal;};
    friend T operator>>(const property<T>& p, T&& o) {o = p.m_internal;};

    // override set
    friend property<T>& operator<<(property<T>& p, const T& o) {p.m_internal = o;};
    friend property<T>& operator<<(property<T>& p, T&& o) {p.m_internal = std::move(o);};

    // casting
    template <typename U, typename T> friend auto property_dynamic_cast(const ext::property<T>& o) -> U;
    template <typename U, typename T> friend auto property_static_cast(const ext::property<T>& o) -> U;
    template <typename U, typename T> friend auto property_const_cast(const ext::property<T>& o) -> U;
    template <typename U, typename T> friend auto property_reinterpret_cast(const ext::property<T>& o) -> U;
    template <typename U, typename T> friend auto property_any_cast(const ext::property<T>& o) -> U;
    template <typename U, typename T> friend auto property_bit_cast(const ext::property<T>& o) -> U;

public: constructors
    property();
    property(const property<T>&) = default;
    property(property<T>&&) noexcept = default;
    auto operator=(const property<T>& o) -> property<T>&;
    auto operator=(property<T>&& o) noexcept -> property<T>&;
    virtual ~property();

public: operators
    virtual operator T() const;
    virtual auto operator=(const T& o) -> property<T>&;
    virtual auto operator=(T&& o) noexcept -> property<T>&;

    template <typename U> operator U() const requires (not std::is_same_v<T, U>);
    template <typename U> auto operator=(const ext::property<U>& o) -> property<T>& requires (not std::is_same_v<T, U>);

    auto operator->() const -> T  requires (std::is_pointer_v<T>);
    auto operator->() const -> T* requires (!std::is_pointer_v<T>);

    template <typename U> auto operator==(const U& o) const -> bool;
    template <typename U> auto operator!=(const U& o) const -> bool;
    template <typename U> auto operator<=(const U& o) const -> bool;
    template <typename U> auto operator>=(const U& o) const -> bool;
    template <typename U> auto operator<(const U& o) const -> bool;
    template <typename U> auto operator>(const U& o) const -> bool;
    template <typename U> auto operator<=>(const U& o) const -> bool;

    template <typename U> auto operator+=(const U& o) -> property<T>&;
    template <typename U> auto operator-=(const U& o) -> property<T>&;
    template <typename U> auto operator*=(const U& o) -> property<T>&;
    template <typename U> auto operator/=(const U& o) -> property<T>&;
    template <typename U> auto operator%=(const U& o) -> property<T>&;
    template <typename U> auto operator^=(const U& o) -> property<T>&;
    template <typename U> auto operator&=(const U& o) -> property<T>&;
    template <typename U> auto operator|=(const U& o) -> property<T>&;
    auto operator<<=(size_t n) -> property<T>& requires (not std::is_arithmetic_v<T>);
    auto operator>>=(size_t n) -> property<T>& requires (not std::is_arithmetic_v<T>);
    auto operator++() -> property<T>&;
    auto operator--() -> property<T>&;

    template <typename U> auto operator+(const U& o) const -> property<T>;
    template <typename U> auto operator-(const U& o) const -> property<T>;
    template <typename U> auto operator*(const U& o) const -> property<T>;
    template <typename U> auto operator/(const U& o) const -> property<T>;
    template <typename U> auto operator%(const U& o) const -> property<T>;
    template <typename U> auto operator^(const U& o) const -> property<T>;
    template <typename U> auto operator&(const U& o) const -> property<T>;
    template <typename U> auto operator|(const U& o) const -> property<T>;
    auto operator<<(size_t n) const -> property<T> requires (not std::is_arithmetic_v<T>);
    auto operator>>(size_t n) const -> property<T> requires (not std::is_arithmetic_v<T>);
    auto operator++(int n) const -> const property<T>;
    auto operator--(int n) const -> const property<T>;

    template <typename U> auto operator&&(const U& o) const -> bool;
    template <typename U> auto operator||(const U& o) const -> bool;

    auto operator+() const -> property<T>;
    auto operator-() const -> property<T>;
    auto operator~() const -> bool;
    auto operator!() const -> bool;
    auto operator*() const -> std::remove_pointer_t<T> requires (std::is_pointer_v<T>);
    auto operator*() const -> T requires (not std::is_pointer_v<T>);

    template <typename U> auto operator[] (size_t i) const -> U&;
    template <typename U, typename ...Args> auto operator() (Args&&... args) const -> U&;
    operator bool() const requires (!std::is_same_v<T, bool>);

public: internal_properties
    std::function<void()> deleter;
    std::function<std::remove_extent_t<T>()> getter;
    std::function<void(std::remove_extent_t<T>)> setter;

private: internal_properties
    T m_internal;
};


template <typename T>
ext::property<T>::property()
{
    // set the deleter, getter and setter functions to the defaults
    deleter = [this]()      {if (std::is_pointer_v<T>) {delete m_internal; m_internal = nullptr;}};
    getter  = [this]()      {return m_internal;};
    setter  = [this](T val) {m_internal = val;};
}


template <typename T>
FAST INLINE auto ext::property<T>::operator=(const property<T>& o) -> ext::property<T>&
{
    // copy another property into this - only transfers the value across, not the accessors
    m_internal = o.m_internal;
    return *this;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator=(property<T>&& o) noexcept -> ext::property<T>&
{
    // move another property into this - only transfers the value across, not the accessors
    m_internal = o.m_internal;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>::~property<T>()
{
    // call the deleter method on destruction, and delete the internal value if it is a pointer
    deleter();
    if constexpr(std::is_pointer_v<T>) {delete m_internal; m_internal = nullptr;}
}


template <typename T>
FAST INLINE ext::property<T>::operator T() const
{
    // conversion to T acts as the getter, so return the value from the get() accessor
    return getter();
}


template<typename T>
FAST INLINE auto ext::property<T>::operator=(const T& o) -> ext::property<T>&
{
    // assignment operator to set an l-value reference, and return the pointer to the property
    setter(o);
    return *this;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator=(T&& o) noexcept -> ext::property<T>&
{
    // assignment operator to set an r-value reference, and return the pointer to the property
    setter(std::forward<T&>(o));
    return *this;
}


template <typename T>
template <typename U>
FAST INLINE ext::property<T>::operator U() const requires (not std::is_same_v<T, U>)
{
    // TODO
    return (U)m_internal;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator=(const ext::property<U>& o) -> ext::property<T>& requires (not std::is_same_v<T, U>)
{
    // TODO
    m_internal = (T)o;
    return *this;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator->() const -> T requires (std::is_pointer_v<T>)
{
    // accessor operator for pointer types needs to return the stored object
    return m_internal;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator->() const -> T* requires (!std::is_pointer_v<T>)
{
    // accessor operator for non-pointer types needs to return the address of the stored object
    return &m_internal;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator==(const U& o) const -> bool
{
    // equality test with internal value
    return m_internal == o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator!=(const U& o) const -> bool
{
    // inequality check with internal value
    return m_internal != o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator<=(const U& o) const -> bool
{
    // less than equal to check with internal value
    return m_internal <= o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator>=(const U& o) const -> bool
{
    // greater than equal to check with internal value
    return m_internal >= o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator<(const U& o) const -> bool
{
    // less than equal to check with internal value
    return m_internal < o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator>(const U& o) const -> bool
{
    // greater than equal to check with internal value
    return m_internal > o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator<=>(const U& o) const -> bool
{
    // less than equal to check with internal value
    return m_internal <=> o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator+=(const U& o) -> ext::property<T>&
{
    // add the value to the internal value, and return the reference to the property
    m_internal += o;
    return *this;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator-=(const U& o) -> ext::property<T>&
{
    // subtract the value to the internal value, and return the reference to the property
    m_internal -= o;
    return *this;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator*=(const U& o) -> ext::property<T>&
{
    // multiply the value to the internal value, and return the reference to the property
    m_internal *= o;
    return *this;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator/=(const U& o) -> ext::property<T>&
{
    // divide the value to the internal value, and return the reference to the property
    m_internal /= o;
    return *this;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator%=(const U& o) -> ext::property<T>&
{
    // the internal value modulo the value, and return the reference to the property
    m_internal %= o;
    return *this;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator^=(const U& o) -> ext::property<T>&
{
    // raise the internal value to the value, and return the reference to the property
    m_internal ^= o;
    return *this;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator&=(const U& o) -> ext::property<T>&
{
    // the internal value bitwise-and the value, and return the reference to the property
    m_internal &= o;
    return *this;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator|=(const U& o) -> ext::property<T>&
{
    // the internal value bitwise-or the value, and return the reference to the property
    m_internal |= o;
    return *this;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator<<=(const size_t n) -> ext::property<T>& requires (not std::is_arithmetic_v<T>)
{
    // left bit-shift the internal value by value, and return the reference to the property
    m_internal <<= n;
    return *this;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator>>=(const size_t n) -> ext::property<T>& requires (not std::is_arithmetic_v<T>)
{
    // right bit-shift the internal value by value, and return the reference to the property
    m_internal >>= n;
    return *this;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator++() -> ext::property<T>&
{
    // increment the internal value and return the reference to the property
    ++m_internal;
    return *this;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator--() -> ext::property<T>&
{
    // decrement the internal value and return the reference to the property
    --m_internal;
    return *this;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator+(const U& o) const -> ext::property<T>
{
    // add the value to the copied internal value, and return the reference to the property
    return property<T>{*this} += o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator-(const U& o) const -> ext::property<T>
{
    // subtract the value to the copied internal value, and return the reference to the property
    return property<T>{*this} -= o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator*(const U& o) const -> ext::property<T>
{
    // multiply the value to the copied internal value, and return the reference to the property
    return property<T>{*this} *= o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator/(const U& o) const -> ext::property<T>
{
    // divide the value to the copied internal value, and return the reference to the property
    return property<T>{*this} /= o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator%(const U& o) const -> ext::property<T>
{
    // the copied internal value modulo the value, and return the reference to the property
    return property<T>{*this} %= o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator^(const U& o) const -> ext::property<T>
{
    // raise the copied internal value to the value, and return the reference to the property
    return property<T>{*this} ^= o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator&(const U& o) const -> ext::property<T>
{
    // the copied internal value bitwise-and the value, and return the reference to the property
    return property<T>{*this} &= o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator|(const U& o) const -> ext::property<T>
{
    // the copied internal value bitwise-or the value, and return the reference to the property
    return property<T>{*this} |= o;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator<<(const size_t n) const -> ext::property<T> requires (not std::is_arithmetic_v<T>)
{
    // left bit-shift the copied internal value by value, and return the reference to the property
    return property<T>{*this} <<= n;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator>>(const size_t n) const -> ext::property<T> requires (not std::is_arithmetic_v<T>)
{
    // right bit-shift the copied internal value by value, and return the reference to the property
    return property<T>{*this} >>= n;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator++(const int n) const -> const ext::property<T>
{
    // increment the internal value by n and return the reference to the property
    return property<T>{*this} += n;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator--(const int n) const -> const ext::property<T>
{
    // decrement the internal value by n and return the reference to the property
    return property<T>{*this} -= n;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator&&(const U& o) const -> bool
{
    // the internal value logical-and the value, and return the boolean evaluation
    return m_internal && o;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator||(const U& o) const -> bool
{
    // the internal value logical-or the value, and return the boolean evaluation
    return m_internal || o;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator+() const -> ext::property<T>
{
    // convert the internal value to positive, and return the reference to the property
    auto copy = ext::property<T>{*this};
    copy.m_internal = +m_internal;
    return copy;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator-() const -> ext::property<T>
{
    // convert the internal value to negative, and return the reference to the property
    auto copy = ext::property<T>{*this};
    copy.m_internal = -m_internal;
    return copy;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator~() const -> bool
{
    // bitwise-not on the internal value
    return ~m_internal;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator!() const -> bool
{
    // logical-not on the internal value
    return !m_internal;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator*() const -> std::remove_pointer_t<T> requires (std::is_pointer_v<T>)
{
    return *m_internal;
}


template <typename T>
FAST INLINE auto ext::property<T>::operator*() const -> T requires (not std::is_pointer_v<T>)
{
    return m_internal;
}


template <typename T>
template <typename U>
FAST INLINE auto ext::property<T>::operator[] (const size_t i) const -> U&
{
    // get the element at index i, and return it
    return m_internal[i];
}


template <typename T>
template <typename U, typename ...Args>
FAST INLINE auto ext::property<T>::operator() (Args&&... args) const -> U&
{
    // return the result of invoking the function stored as the internal value
    return m_internal(std::forward<Args...>(args...));
}


template <typename T>
FAST INLINE ext::property<T>::operator bool() const requires (!std::is_same_v<T, bool>)
{
    // convert any non-property2<bool> property2<T> into a boolean and return the evaluation
    return (bool)m_internal;
}


template <typename U, typename T>
auto ext::property_dynamic_cast(const ext::property<T>& o) -> U
{
    // dynamic_cast the internal value into a new value
    return dynamic_cast<U>(o.m_internal);
}


template <typename U, typename T>
auto ext::property_static_cast(const ext::property<T>& o) -> U
{
    // static_cast the internal value into a new value
    return static_cast<U>(o.m_internal);
}


template <typename U, typename T>
auto ext::property_const_cast(const ext::property<T>& o) -> U
{
    // const_cast the internal value into a new value
    return const_cast<U>(o.m_internal);
}


template <typename U, typename T>
auto ext::property_reinterpret_cast(const ext::property<T>& o) -> U
{
    // reinterpret_cast the internal value into a new value
    return reinterpret_cast<U>(o.m_internal);
}


template <typename U, typename T>
auto ext::property_any_cast(const ext::property<T>& o) -> U
{
    // any_cast the internal value into a new value
    return any_cast<U>(o.m_internal);
}


template <typename U, typename T>
auto ext::property_bit_cast(const ext::property<T>& o) -> U
{
    // bit_cast the internal value into a new value
    return std::bit_cast<U>(o);
}


#endif //SBROWSER_PROPERTY_HPP
