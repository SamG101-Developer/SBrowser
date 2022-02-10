#ifndef SBROWSER_PROPERTY2_HPP
#define SBROWSER_PROPERTY2_HPP

#include <functional>
#include <type_traits>

#include <ext/macros/cpp_keywords.hpp>

namespace ext {
    template <typename T> struct property2;
    template <typename T> struct dom_property2;
    template <typename T> struct html_property2;
    struct css_property2;
    struct css_longhand_property2;

    template <typename U, typename T> U property_dynamic_cast(const ext::property2<T>& o);
    template <typename U, typename T> U property_static_cast(const ext::property2<T>& o);
    template <typename U, typename T> U property_const_cast(const ext::property2<T>& o);
    template <typename U, typename T> U property_reinterpret_cast(const ext::property2<T>& o);
    template <typename U, typename T> U property_any_cast(const ext::property2<T>& o);
}


template <typename T>
struct ext::property2 {
    // override get
    friend T operator>>(const property2<T>& p, T& o);
    friend T operator>>(const property2<T>& p, T&& o);

    // override set
    friend property2<T>& operator<<(property2<T>& p, const T& o);
    friend property2<T>& operator<<(property2<T>& p, T&& o);

    // casting
    template <typename U, typename T> friend U property_dynamic_cast(const ext::property2<T>& o);
    template <typename U, typename T> friend U property_static_cast(const ext::property2<T>& o);
    template <typename U, typename T> friend U property_const_cast(const ext::property2<T>& o);
    template <typename U, typename T> friend U property_reinterpret_cast(const ext::property2<T>& o);
    template <typename U, typename T> friend U property_any_cast(const ext::property2<T>& o);

public:
    property2();
    property2(T val);
    property2(const property2<T>&) = default;
    property2(property2<T>&&) noexcept = default;
    property2<T>& operator=(const property2<T>& o);
    property2<T>& operator=(property2<T>&& o) noexcept;
    virtual ~property2();

    operator T() const;
    property2<T>& operator=(const T& o);
    property2<T>& operator=(T&& o) noexcept;
    template<typename U> property2<T>& operator=(const ext::property2<U>& o);

    T* operator->() const requires (!std::is_pointer_v<T>);
    T operator->() const requires (std::is_pointer_v<T>);

    bool operator==(const T& o) const;
    bool operator!=(const T& o) const;
    bool operator<=(const T& o) const;
    bool operator>=(const T& o) const;
    bool operator<(const T& o) const;
    bool operator>(const T& o) const;
    bool operator<=>(const T& o) const;

    property2<T>& operator+=(const T& o);
    property2<T>& operator-=(const T& o);
    property2<T>& operator*=(const T& o);
    property2<T>& operator/=(const T& o);
    property2<T>& operator%=(const T& o);
    property2<T>& operator^=(const T& o);
    property2<T>& operator&=(const T& o);
    property2<T>& operator|=(const T& o);
    property2<T>& operator<<=(const size_t n);
    property2<T>& operator>>=(const size_t n);
    property2<T>& operator++();
    property2<T>& operator--();

    property2<T> operator+(const T& o) const;
    property2<T> operator-(const T& o) const;
    property2<T> operator*(const T& o) const;
    property2<T> operator/(const T& o) const;
    property2<T> operator%(const T& o) const;
    property2<T> operator^(const T& o) const;
    property2<T> operator&(const T& o) const;
    property2<T> operator|(const T& o) const;
    property2<T> operator<<(const size_t n) const;
    property2<T> operator>>(const size_t n) const;
    property2<T> operator++(const int n) const;
    property2<T> operator--(const int n) const;
    bool operator&&(const T& o) const;
    bool operator||(const T& o) const;

    property2<T> operator+() const;
    property2<T> operator-() const;
    property2<T> operator~() const;
    bool operator!() const;

    T& operator[] (const size_t i) const;
    template <class ...Args> auto operator() (Args... args) const;
    operator bool() const requires (!std::is_same_v<T, bool>);

public:
    std::function<void()> del;
    std::function<T()> get;
    std::function<void(T)> set;

private:
    T m_internal;
};


template <typename T>
ext::property2<T>::property2()
{
    // set the deleter, getter and setter functions to the defaults
    del = [this]()      -> void {};
    get = [this]()      -> T {return m_internal;};
    set = [this](T val) -> void {m_internal = val;};
}


template <typename T>
FAST INLINE ext::property2<T>::property2(T val) : property2()
{
    // set the internal value directly as the initialization value
    m_internal = val;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator=(const property2<T>& o)
{
    // copy another property into this - only transfers the value across, not the accessors
    m_internal = o.m_internal;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator=(property2<T>&& o) noexcept
{
    // move another property into this - only transfers the value across, not the accessors
    m_internal = o.m_internal;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>::~property2<T>()
{
    // call the deleter method on destruction, and delete the internal value if it is a pointer
    del();
    if constexpr(std::is_pointer_v<T>) delete m_internal;
}


template <typename T>
FAST INLINE ext::property2<T>::operator T() const
{
    // conversion to T acts as the getter, so return the value from the get() accessor
    return get();
}


template<typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator=(const T& o)
{
    // assignment operator to set an l-value reference, and return the pointer to the property
    set(o);
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator=(T&& o) noexcept
{
    // assignment operator to set an r-value reference, and return the pointer to the property
    set(std::forward<T&>(o));
    return *this;
}


template <typename T>
FAST INLINE T* ext::property2<T>::operator->() const requires (!std::is_pointer_v<T>)
{
    // accessor operator for non-pointer types needs to return the address of the stored object
    return &m_internal;
}


template <typename T>
FAST INLINE T ext::property2<T>::operator->() const requires (std::is_pointer_v<T>)
{
    // accessor operator for pointer types needs to return the stored object
    return m_internal;
}


template <typename T>
FAST INLINE bool ext::property2<T>::operator==(const T& o) const {

    // equality test with internal value
    return m_internal == o;
}


template <typename T>
FAST INLINE bool ext::property2<T>::operator!=(const T& o) const {

    // inequality check with internal value
    return m_internal != o;
}


template <typename T>
FAST INLINE bool ext::property2<T>::operator<=(const T& o) const {

    // less than equal to check with internal value
    return m_internal <= o;
}


template <typename T>
FAST INLINE bool ext::property2<T>::operator>=(const T& o) const {

    // greater than equal to check with internal value
    return m_internal >= o;
}


template <typename T>
FAST INLINE bool ext::property2<T>::operator<(const T& o) const {

    // less than equal to check with internal value
    return m_internal < o;
}


template <typename T>
FAST INLINE bool ext::property2<T>::operator>(const T& o) const {

    // greater than equal to check with internal value
    return m_internal > o;
}


template <typename T>
FAST INLINE bool ext::property2<T>::operator<=>(const T& o) const {

    // less than equal to check with internal value
    return m_internal <=> o;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator+=(const T& o) {

    // add the value to the internal value, and return the reference to the property
    m_internal += o;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator-=(const T& o) {

    // subtract the value to the internal value, and return the reference to the property
    m_internal -= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator*=(const T& o) {

    // multiply the value to the internal value, and return the reference to the property
    m_internal *= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator/=(const T& o) {

    // divide the value to the internal value, and return the reference to the property
    m_internal /= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator%=(const T& o) {

    // the internal value modulo the value, and return the reference to the property
    m_internal %= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator^=(const T& o) {

    // raise the internal value to the value, and return the reference to the property
    m_internal ^= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator&=(const T& o) {

    // the internal value bitwise-and the value, and return the reference to the property
    m_internal &= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator|=(const T& o) {

    // the internal value bitwise-or the value, and return the reference to the property
    m_internal |= o;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator<<=(const size_t n) {

    // left bit-shift the internal value by value, and return the reference to the property
    m_internal <<= n;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator>>=(const size_t n) {

    // right bit-shift the internal value by value, and return the reference to the property
    m_internal >>= n;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator++() {

    // increment the internal value and return the reference to the property
    ++m_internal;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T>& ext::property2<T>::operator--() {

    // decrement the internal value and return the reference to the property
    --m_internal;
    return *this;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator+(const T& o) const {

    // add the value to the copied internal value, and return the reference to the property
    return property2<T>{*this} += o;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator-(const T& o) const {

    // subtract the value to the copied internal value, and return the reference to the property
    return property2<T>{*this} -= o;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator*(const T& o) const {

    // multiply the value to the copied internal value, and return the reference to the property
    return property2<T>{*this} *= o;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator/(const T& o) const {

    // divide the value to the copied internal value, and return the reference to the property
    return property2<T>{*this} /= o;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator%(const T& o) const {

    // the copied internal value modulo the value, and return the reference to the property
    return property2<T>{*this} %= o;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator^(const T& o) const {

    // raise the copied internal value to the value, and return the reference to the property
    return property2<T>{*this} ^= o;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator&(const T& o) const {

    // the copied internal value bitwise-and the value, and return the reference to the property
    return property2<T>{*this} &= o;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator|(const T& o) const {

    // the copied internal value bitwise-or the value, and return the reference to the property
    return property2<T>{*this} |= o;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator<<(const size_t n) const {

    // left bit-shift the copied internal value by value, and return the reference to the property
    return property2<T>{*this} <<= n;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator>>(const size_t n) const {

    // right bit-shift the copied internal value by value, and return the reference to the property
    return property2<T>{*this} >>= n;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator++(const int n) const {

    // increment the internal value by n and return the reference to the property
    return property2<T>{*this} += n;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator--(const int n) const {

    // decrement the internal value by n and return the reference to the property
    return property2<T>{*this} -= n;
}


template <typename T>
FAST INLINE bool ext::property2<T>::operator&&(const T& o) const {

    // the internal value logical-and the value, and return the boolean evaluation
    return m_internal && o;
}


template <typename T>
FAST INLINE bool ext::property2<T>::operator||(const T& o) const {

    // the internal value logical-or the value, and return the boolean evaluation
    return m_internal || o;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator+() const {

    // convert the internal value to positive, and return the reference to the property
    auto copy = ext::property2<T>{*this};
    copy.m_internal = +m_internal;
    return copy;
}


template <typename T>
FAST INLINE ext::property2<T> ext::property2<T>::operator-() const {

    // convert the internal value to negative, and return the reference to the property
    auto copy = ext::property2<T>{*this};
    copy.m_internal = -m_internal;
    return copy;
}



#endif //SBROWSER_PROPERTY2_HPP
