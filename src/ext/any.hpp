#ifndef SBROWSER_ANY_HPP
#define SBROWSER_ANY_HPP

namespace ext {class any;}

#include <any>
#include <string>

#include <ext/cpp_keywords.hpp>
#include <ext/decorators.hpp>


class ext::any final
{
public constructors:
    any() = default;
    ~any() = default;

    any(const any&) = default;
    any(any&&) noexcept = default;
    auto operator=(const any&) -> any& = default;
    auto operator=(any&&) noexcept -> any& = default;

    template <typename T> any(const T& that);
    template <typename T> any(T&& that) noexcept;
    template <typename T> auto operator=(const T& that) -> any&;
    template <typename T> auto operator=(T&& that) noexcept -> any&;

public operators:
    auto operator==(const ext::any& that) const -> bool;

public cpp_methods:
    // type information
    [[nodiscard]] auto type() const -> const type_info&;
    [[nodiscard]] auto is_numeric() const -> bool;

    // state information / modification
    [[nodiscard]] auto empty() const -> bool;
    template <typename T> auto to() const -> T&;

private cpp_methods:
    template <typename T> [[nodiscard]] auto any_cast() const -> T;

private cpp_properties:
    std::any m_value;
};


template <typename T>
ext::any::any(const T& that)
{
    // use the constructor to emplace the copied object into this any value
    m_value.template emplace<T>(that);
}


template <typename T>
ext::any::any(T&& that) noexcept
{
    // use the constructor to emplace the moved object into this optional value
    m_value.template emplace<T>(std::forward<T>(that));
}


template <typename T>
auto ext::any::operator=(
        const T& that)
        -> any&
{
    // use the assignment operator to emplace the copied object into this optional value
    m_value.template emplace<T>(that);
}


template <typename T>
auto ext::any::operator=(
        T&& that) noexcept
        -> any&
{
    // use the assignment operator to emplace the copied object into this optional value
    m_value.template emplace<T>(std::forward<T>(that));
}


auto ext::any::operator==(const ext::any& that) const -> bool
{
    return &m_value == &that.m_value;
}


auto ext::any::type() const
        -> const type_info&
{
    // return the type of the internal object wrapped with the ext::any
    return m_value.type();
}


auto ext::any::empty() const
        -> bool
{
    // return if the ext::any object is empty ie there is no internal object wrapped
    return not m_value.has_value();
}


auto ext::any::is_numeric() const
        -> bool
{
    // return is the internal type is numeric
    TRY
        return static_cast<bool>(any_cast<int>());
    CATCH(const std::bad_any_cast)
        return false;
    END_TRY
}


template <typename T>
auto ext::any::to() const
        -> T&
{
    // cast the internal object wrapped to any type T
    return any_cast<T>(m_value);
}


template <class T>
auto ext::any::any_cast() const
        -> T
{
    // if the value is empty or a void, then return a nullptr for pointer types, otherwise a new stack object of type T
    // ie 0 for an int or "" for a string etc
    if (empty() or type() == typeid(void))
    {
        if constexpr(std::is_pointer_v<T>)
            return nullptr;
        return T();
    }

    // otherwise, return the internal object unwrapped, as type T
    return std::any_cast<T>(m_value);
}


#endif //SBROWSER_ANY_HPP
