#ifndef SBROWSER_OPTIONAL_HPP
#define SBROWSER_OPTIONAL_HPP

#include <optional>

namespace ext {template <typename T> class optional;}
namespace {template <typename T> T optional_cast(ext::optional<T> value);}


template <typename T>
class ext::optional
{
public constructors:
    optional();
    optional(const optional<T>&) = delete;
    optional(optional<T>&&) noexcept = delete;
    optional<T>& operator=(const optional<T>&) = delete;
    optional<T>& operator=(optional<T>&) noexcept = delete;

    optional<T>& operator=(const T& other);
    optional<T>& operator=(T&& other) noexcept;

public methods:
    bool empty() const;

public operators:
    operator bool() const;
    operator T() const;

private internal_properties:
    std::optional<T> m_value;
};


template<typename T>
ext::optional<T>::optional()
{
    // set the default value of the optional value to null
    m_value = std::nullopt;
}


template<typename T>
ext::optional<T>& ext::optional<T>::operator=(const T& other)
{
    // use the assignment operator to emplace the copied object into this optional value
    m_value.template emplace(other);
}


template <typename T>
ext::optional<T>& ext::optional<T>::operator=(T&& other) noexcept
{
    // use the assignment operator to emplace the moved object into this optional value
    m_value.template emplace(std::move(other));
}


template <typename T>
bool ext::optional<T>::empty() const
{
    // check if the optional value is empty or not
    return not m_value.has_value() or m_value == std::nullopt;
}


template <typename T>
ext::optional<T>::operator T() const
{
    // get the optional value if it exists
    return optional_cast<T>(this);
}


template <typename T>
ext::optional<T>::operator bool() const
{
    // the boolean conversion checks if the optional value is not empty
    return not empty();
}


template<typename T>
T optional_cast(ext::optional<T> value)
{
    // return the internal value if there is one, otherwise throw an error
    if (value)
        return value.m_value;
    throw std::bad_variant_access();
}


#endif //SBROWSER_OPTIONAL_HPP
