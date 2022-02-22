#ifndef SBROWSER_PROPERTY_HPP
#define SBROWSER_PROPERTY_HPP

#include <functional>
#include <type_traits>

#include <ext/macros/cpp_keywords.hpp>
#include <ext/macros/decorators.hpp>
#include <ext/types/any.hpp>

namespace ext {
    template <typename T> struct property;

    template <typename U, typename T> U property_dynamic_cast(const ext::property<T>& o);
    template <typename U, typename T> U property_static_cast(const ext::property<T>& o);
    template <typename U, typename T> U property_const_cast(const ext::property<T>& o);
    template <typename U, typename T> U property_reinterpret_cast(const ext::property<T>& o);
    template <typename U, typename T> U property_any_cast(const ext::property<T>& o);
    template <typename U, typename T> U property_bit_cast(const ext::property<T>& o);
}


template <typename T>
struct ext::property {
public: friends
    // override get
    friend T operator>>(const property<T>& p, T& o);
    friend T operator>>(const property<T>& p, T&& o);

    // override set
    friend property<T>& operator<<(property<T>& p, const T& o);
    friend property<T>& operator<<(property<T>& p, T&& o);

    // casting
    template <typename U, typename T> friend U property_dynamic_cast(const ext::property<T>& o);
    template <typename U, typename T> friend U property_static_cast(const ext::property<T>& o);
    template <typename U, typename T> friend U property_const_cast(const ext::property<T>& o);
    template <typename U, typename T> friend U property_reinterpret_cast(const ext::property<T>& o);
    template <typename U, typename T> friend U property_any_cast(const ext::property<T>& o);
    template <typename U, typename T> friend U property_bit_cast(const ext::property<T>& o);

public: constructors
    property();
    property(const property<T>&) = default;
    property(property<T>&&) noexcept = default;
    property<T>& operator=(const property<T>& o);
    property<T>& operator=(property<T>&& o) noexcept;
    virtual ~property();

    virtual operator T() const;
    virtual property<T>& operator=(const T& o);
    virtual property<T>& operator=(T&& o) noexcept;

    template <typename U> operator U() const requires (not std::is_same_v<T, U>);
    template <typename U> property<T>& operator=(const ext::property<U>& o) requires (not std::is_same_v<T, U>);

    T* operator->() const requires (!std::is_pointer_v<T>);
    T operator->() const requires (std::is_pointer_v<T>);

    bool operator==(const T& o) const;
    bool operator!=(const T& o) const;
    bool operator<=(const T& o) const;
    bool operator>=(const T& o) const;
    bool operator<(const T& o) const;
    bool operator>(const T& o) const;
    bool operator<=>(const T& o) const;

    property<T>& operator+=(const T& o);
    property<T>& operator-=(const T& o);
    property<T>& operator*=(const T& o);
    property<T>& operator/=(const T& o);
    property<T>& operator%=(const T& o);
    property<T>& operator^=(const T& o);
    property<T>& operator&=(const T& o);
    property<T>& operator|=(const T& o);
    property<T>& operator<<=(const size_t n) requires (not std::is_arithmetic_v<T>);
    property<T>& operator>>=(const size_t n) requires (not std::is_arithmetic_v<T>);
    property<T>& operator++();
    property<T>& operator--();

    property<T> operator+(const T& o) const;
    property<T> operator-(const T& o) const;
    property<T> operator*(const T& o) const;
    property<T> operator/(const T& o) const;
    property<T> operator%(const T& o) const;
    property<T> operator^(const T& o) const;
    property<T> operator&(const T& o) const;
    property<T> operator|(const T& o) const;
    property<T> operator<<(const size_t n) const requires (not std::is_arithmetic_v<T>);
    property<T> operator>>(const size_t n) const requires (not std::is_arithmetic_v<T>);
    property<T> operator++(const int n) const;
    property<T> operator--(const int n) const;

    bool operator&&(const T& o) const;
    bool operator||(const T& o) const;

    property<T> operator+() const;
    property<T> operator-() const;
    bool operator~() const;
    bool operator!() const;
    std::remove_pointer_t<T> operator*() const requires (std::is_pointer_v<T>);

    template <typename U> U& operator[] (const size_t i) const;
    template <typename U, typename ...Args> U& operator() (Args&&... args) const;
    operator bool() const requires (!std::is_same_v<T, bool>);

public: internal_properties
    std::function<void()> del;
    std::function<T()> get;
    std::function<void(T)> set;

private: internal_properties
    T m_internal;
};


template <typename T>
ext::property<T>::property()
{
    // set the deleter, getter and setter functions to the defaults
    del = [this]()      -> void {};
    get = [this]()      -> T {return m_internal;};
    set = [this](T val) -> void {m_internal = val;};
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator=(const property<T>& o)
{
    // copy another property into this - only transfers the value across, not the accessors
    m_internal = o.m_internal;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator=(property<T>&& o) noexcept
{
    // move another property into this - only transfers the value across, not the accessors
    m_internal = o.m_internal;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>::~property<T>()
{
    // call the deleter method on destruction, and delete the internal value if it is a pointer
    del();
    if constexpr(std::is_pointer_v<T>) delete m_internal;
}


template <typename T>
FAST INLINE ext::property<T>::operator T() const
{
    // conversion to T acts as the getter, so return the value from the get() accessor
    return get();
}


template<typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator=(const T& o)
{
    // assignment operator to set an l-value reference, and return the pointer to the property
    set(o);
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator=(T&& o) noexcept
{
    // assignment operator to set an r-value reference, and return the pointer to the property
    set(std::forward<T&>(o));
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
FAST INLINE ext::property<T>& ext::property<T>::operator=(const ext::property<U>& o) requires (not std::is_same_v<T, U>)
{
    // TODO
    m_internal = (T)o;
    return *this;
}



template <typename T>
FAST INLINE T* ext::property<T>::operator->() const requires (!std::is_pointer_v<T>)
{
    // accessor operator for non-pointer types needs to return the address of the stored object
    return &m_internal;
}


template <typename T>
FAST INLINE T ext::property<T>::operator->() const requires (std::is_pointer_v<T>)
{
    // accessor operator for pointer types needs to return the stored object
    return m_internal;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator==(const T& o) const
{
    // equality test with internal value
    return m_internal == o;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator!=(const T& o) const
{
    // inequality check with internal value
    return m_internal != o;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator<=(const T& o) const
{
    // less than equal to check with internal value
    return m_internal <= o;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator>=(const T& o) const
{
    // greater than equal to check with internal value
    return m_internal >= o;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator<(const T& o) const
{
    // less than equal to check with internal value
    return m_internal < o;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator>(const T& o) const
{
    // greater than equal to check with internal value
    return m_internal > o;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator<=>(const T& o) const
{
    // less than equal to check with internal value
    return m_internal <=> o;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator+=(const T& o)
{
    // add the value to the internal value, and return the reference to the property
    m_internal += o;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator-=(const T& o)
{
    // subtract the value to the internal value, and return the reference to the property
    m_internal -= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator*=(const T& o)
{
    // multiply the value to the internal value, and return the reference to the property
    m_internal *= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator/=(const T& o)
{
    // divide the value to the internal value, and return the reference to the property
    m_internal /= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator%=(const T& o)
{
    // the internal value modulo the value, and return the reference to the property
    m_internal %= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator^=(const T& o)
{
    // raise the internal value to the value, and return the reference to the property
    m_internal ^= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator&=(const T& o)
{
    // the internal value bitwise-and the value, and return the reference to the property
    m_internal &= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator|=(const T& o)
{
    // the internal value bitwise-or the value, and return the reference to the property
    m_internal |= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator<<=(const size_t n) requires (not std::is_arithmetic_v<T>)
{
    // left bit-shift the internal value by value, and return the reference to the property
    m_internal <<= n;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator>>=(const size_t n) requires (not std::is_arithmetic_v<T>)
{
    // right bit-shift the internal value by value, and return the reference to the property
    m_internal >>= n;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator++()
{
    // increment the internal value and return the reference to the property
    ++m_internal;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T>& ext::property<T>::operator--()
{
    // decrement the internal value and return the reference to the property
    --m_internal;
    return *this;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator+(const T& o) const
{
    // add the value to the copied internal value, and return the reference to the property
    return property<T>{*this} += o;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator-(const T& o) const
{
    // subtract the value to the copied internal value, and return the reference to the property
    return property<T>{*this} -= o;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator*(const T& o) const
{
    // multiply the value to the copied internal value, and return the reference to the property
    return property<T>{*this} *= o;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator/(const T& o) const
{
    // divide the value to the copied internal value, and return the reference to the property
    return property<T>{*this} /= o;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator%(const T& o) const
{
    // the copied internal value modulo the value, and return the reference to the property
    return property<T>{*this} %= o;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator^(const T& o) const
{
    // raise the copied internal value to the value, and return the reference to the property
    return property<T>{*this} ^= o;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator&(const T& o) const
{
    // the copied internal value bitwise-and the value, and return the reference to the property
    return property<T>{*this} &= o;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator|(const T& o) const
{
    // the copied internal value bitwise-or the value, and return the reference to the property
    return property<T>{*this} |= o;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator<<(const size_t n) const requires (not std::is_arithmetic_v<T>)
{
    // left bit-shift the copied internal value by value, and return the reference to the property
    return property<T>{*this} <<= n;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator>>(const size_t n) const requires (not std::is_arithmetic_v<T>)
{
    // right bit-shift the copied internal value by value, and return the reference to the property
    return property<T>{*this} >>= n;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator++(const int n) const
{
    // increment the internal value by n and return the reference to the property
    return property<T>{*this} += n;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator--(const int n) const
{
    // decrement the internal value by n and return the reference to the property
    return property<T>{*this} -= n;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator&&(const T& o) const
{
    // the internal value logical-and the value, and return the boolean evaluation
    return m_internal && o;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator||(const T& o) const
{
    // the internal value logical-or the value, and return the boolean evaluation
    return m_internal || o;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator+() const
{
    // convert the internal value to positive, and return the reference to the property
    auto copy = ext::property<T>{*this};
    copy.m_internal = +m_internal;
    return copy;
}


template <typename T>
FAST INLINE ext::property<T> ext::property<T>::operator-() const
{
    // convert the internal value to negative, and return the reference to the property
    auto copy = ext::property<T>{*this};
    copy.m_internal = -m_internal;
    return copy;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator~() const
{
    // bitwise-not on the internal value
    return ~m_internal;
}


template <typename T>
FAST INLINE bool ext::property<T>::operator!() const
{
    // logical-not on the internal value
    return !m_internal;
}


template <typename T>
FAST INLINE std::remove_pointer_t<T> ext::property<T>::operator*() const requires (std::is_pointer_v<T>)
{
    return *m_internal;
}


template <typename T>
template <typename U>
FAST INLINE U& ext::property<T>::operator[] (const size_t i) const
{
    // get the element at index i, and return it
    return m_internal[i];
}


template <typename T>
template <typename U, typename ...Args>
FAST INLINE U& ext::property<T>::operator() (Args&&... args) const
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
U ext::property_dynamic_cast(const ext::property<T>& o)
{
    // dynamic_cast the internal value into a new value
    return dynamic_cast<U>(o.m_internal);
}


template <typename U, typename T>
U ext::property_static_cast(const ext::property<T>& o)
{
    // static_cast the internal value into a new value
    return static_cast<U>(o.m_internal);
}


template <typename U, typename T>
U ext::property_const_cast(const ext::property<T>& o)
{
    // const_cast the internal value into a new value
    return const_cast<U>(o.m_internal);
}


template <typename U, typename T>
U ext::property_reinterpret_cast(const ext::property<T>& o)
{
    // reinterpret_cast the internal value into a new value
    return reinterpret_cast<U>(o.m_internal);
}


template <typename U, typename T>
U ext::property_any_cast(const ext::property<T>& o)
{
    // any_cast the internal value into a new value
    return any_cast<U>(o.m_internal);
}


template <typename U, typename T>
U ext::property_bit_cast(const ext::property<T>& o)
{
    // bit_cast the internal value into a new value
    return std::bit_cast<U>(o);
}


#endif //SBROWSER_PROPERTY_HPP
