#ifndef SBROWSER_PROPERTY2_HPP
#define SBROWSER_PROPERTY2_HPP

#include <functional>
#include <type_traits>

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

    property2<T>& operator+=(const T& o);
    property2<T>& operator-=(const T& o);
    property2<T>& operator*=(const T& o);
    property2<T>& operator/=(const T& o);
    property2<T>& operator%=(const T& o);
    property2<T>& operator^=(const T& o);
    property2<T>& operator&=(const T& o);
    property2<T>& operator|=(const T& o);
    property2<T>& operator<<=(const size_t n);
    property2<T>& operator++();
    property2<T>& operator--();

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
ext::property2<T>::property2(T val) : property2()
{
    // set the internal value directly as the initialization value
    m_internal = val;
}


template <typename T>
ext::property2<T>& ext::property2<T>::operator=(const property2<T>& o)
{
    // copy another property into this - only transfers the value across, not the accessors
    m_internal = o.m_internal;
    return *this;
}


template <typename T>
ext::property2<T>& ext::property2<T>::operator=(property2<T>&& o) noexcept
{
    // move another property into this - only transfers the value across, not the accessors
    m_internal = o.m_internal;
    return *this;
}


template <typename T>
ext::property2<T>::~property2<T>()
{
    // call the deleter method on destruction, and delete the internal value if it is a pointer
    del();
    if constexpr(std::is_pointer_v<T>) delete m_internal;
}


template <typename T>
ext::property2<T>::operator T() const
{
    // conversion to T acts as the getter, so return the value from the get() accessor
    return get();
}


template<typename T>
ext::property2<T>& ext::property2<T>::operator=(const T& o)
{
    // assignment operator to set an l-value reference, and return the pointer to the property
    set(o);
    return *this;
}


template <typename T>
ext::property2<T>& ext::property2<T>::operator=(T&& o) noexcept
{
    // assignment operator to set an r-value reference, and return the pointer to the property
    set(std::forward<T&>(o));
    return *this;
}


template <typename T>
T* ext::property2<T>::operator->() const requires (!std::is_pointer_v<T>)
{
    // accessor operator for non-pointer types needs to return the address of the stored object
    return &m_internal;
}


template <typename T>
T ext::property2<T>::operator->() const requires (std::is_pointer_v<T>)
{
    // accessor operator for pointer types needs to return the stored object
    return m_internal;
}


#endif //SBROWSER_PROPERTY2_HPP
