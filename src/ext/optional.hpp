#ifndef SBROWSER_OPTIONAL_HPP
#define SBROWSER_OPTIONAL_HPP

namespace ext {template <typename T> class optional;}

#include <optional>
#include <ext/type_traits.hpp>


inline constexpr std::nullopt_t null{std::nullopt_t::_Tag{}};


template <typename T>
class ext::optional final
{
public constructors:
    optional() = default;
    optional(const optional&) = delete;
    optional(optional&&) noexcept = delete;
    auto operator=(const optional&) -> optional& = delete;
    auto operator=(optional&&) noexcept -> optional& = delete;
    virtual ~optional() = default;

    explicit optional(T&& value) noexcept;
    auto operator=(T&& other) noexcept -> optional&;
    auto operator=(std::nullopt_t nullopt) -> optional&;

public js_methods:
    [[nodiscard]] constexpr auto empty() const -> bool;
    [[nodiscard]] constexpr auto has_value() const -> bool;
    constexpr auto value() const -> T&;
    constexpr auto value_or(const T& other) const -> T&;
    constexpr auto not_value_or(const T& other) const -> optional&;

private cpp_properties:
    std::optional<T> m_value;
};


template <typename T>
ext::optional<T>::optional(
        T&& value) noexcept
{
    // use the constructor to emplace the moved object into this optional value
    m_value.template emplace(std::forward<T>(value));
}


template <typename T>
auto ext::optional<T>::operator=(
        T&& other) noexcept
        -> optional&
{
    // use the assignment operator to emplace the moved object into this optional value
    m_value.template emplace(std::forward<T>(other));
    return *this;
}


template <typename T>
auto ext::optional<T>::operator=(
        std::nullopt_t nullopt)
        -> optional&
{
    m_value = nullopt;
    return *this;
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
        -> optional&
{
    // return null if there is no value, otherwise the backup value
    return not has_value() ? null : other;
}


#endif //SBROWSER_OPTIONAL_HPP
