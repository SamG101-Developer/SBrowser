#ifndef SBROWSER_OPTIONAL_HPP
#define SBROWSER_OPTIONAL_HPP

#include <optional>
#include <ext/type_traits.hpp>

namespace ext {template <typename T> class optional;}


template <typename T>
class ext::optional
{
public constructors:
    optional() = default;
    virtual ~optional() = default;

    optional(const optional&) = delete;
    optional(optional&&) noexcept = delete;
    auto operator=(const optional&) -> optional& = delete;
    auto operator=(optional&&) noexcept -> optional& = delete;

    explicit optional(const T& value);
    explicit optional(T&& value) noexcept;
    auto operator=(const T& other) -> optional&;
    auto operator=(T&& other) noexcept -> optional&;

public js_methods:
    constexpr auto empty() const -> bool;
    constexpr auto has_value() const -> bool;
    constexpr auto value() const -> T&;
    constexpr auto value_or(const T& other) const -> T&;
    constexpr auto not_value_or(const T& other) const -> ext::optional<T&>;

private cpp_properties:
    std::optional<T> m_value;
};


template <typename T>
ext::optional<T>::optional(
        const T& value)
{
    // use the constructor to emplace the copied object into this optional value
    m_value.template emplace(value);
}


template <typename T>
ext::optional<T>::optional(
        T&& value) noexcept
{
    // use the constructor to emplace the moved object into this optional value
    m_value.template emplace(std::forward<T&>(value));
}


template <typename T>
auto ext::optional<T>::operator=(
        const T& other)
        -> ext::optional<T>&
{
    // use the assignment operator to emplace the copied object into this optional value
    m_value.template emplace(other);
}


template <typename T>
auto ext::optional<T>::operator=(
        T&& other) noexcept
        -> ext::optional<T>&
{
    // use the assignment operator to emplace the moved object into this optional value
    m_value.template emplace(std::forward<T&>(other));
}


template <typename T>
constexpr auto ext::optional<T>::empty() const
        -> bool
{
    // check if the optional value is empty or not
    return not m_value.has_value() or m_value == null;
}


template <typename T>
constexpr auto ext::optional<T>::has_value() const
        -> bool
{
    // check if the optional value is empty or not
    return m_value.has_value();
}


template <typename T>
constexpr auto ext::optional<T>::value() const
        -> T&
{
    // get the internal value if there is one
    if (has_value())
        return *m_value;

    // if there is no value, throw an error
    throw std::bad_optional_access("No value stored in the optional object (" + std::to_string(this) + ")");
}


template <typename T>
constexpr auto ext::optional<T>::value_or(
        const T& other) const
        -> T&
{
    // return the value if it exists, otherwise the backup value
    return has_value() ? *m_value : other;
}


template <typename T>
constexpr auto ext::optional<T>::not_value_or(
        const T& other) const
        -> ext::optional<T&>
{
    // return null if there is no value, otherwise the backup value
    return not has_value() ? null : other;
}


#endif //SBROWSER_OPTIONAL_HPP
