#ifndef SBROWSER_ANY_HPP
#define SBROWSER_ANY_HPP

#include <any>

#include <ext/string.hpp>
#include <ext/decorators.hpp>


//inline bool operator< (const std::any& first, const std::any& second) {return &first <  &second;}
//inline bool operator> (const std::any& first, const std::any& second) {return &first >  &second;}
//inline bool operator==(const std::any& first, const std::any& second) {return &first == &second;}


namespace ext {class any;}
namespace {template <class T> T any_cast(ext::any value);}


class ext::any
{
public constructors:
    any() = default;
    any(const any&) = default;
    any(any&&) noexcept = default;
    auto operator=(const any&) -> any& = default;
    auto operator=(any&&) noexcept -> any& = default;

    template <typename T> any(const T& other) {m_value = other;};
    template <typename T> any(T&& other) noexcept {m_value = other;};
    template <typename T> auto operator=(const T& other) -> any& {m_value = other; return *this;}
    template <typename T> auto operator=(T&& other) noexcept -> any& {m_value = std::forward<T&>(other); return *this;}

public js_methods:
    auto type() const -> const type_info&;
    auto empty() const -> bool;
    auto contains_pointer() const -> bool;
    auto is_numeric() const -> bool;
    template <typename T> auto emplace() -> any&;
    template <typename T> auto emplace(T&& element) -> any&;
    template <typename T> auto to() const -> T;

public operators:
    operator bool() const;

private cpp_properties:
    std::any m_value;
};


auto ext::any::type() const -> const type_info&
{
    // return the type of the internal object wrapped with the ext::any
    return m_value.type();
}


auto ext::any::empty() const -> bool
{
    // return if the ext::any object is empty ie there is no internal object wrapped
    return not m_value.has_value();
}


auto ext::any::contains_pointer() const -> bool
{
    // return if the internal object wrapped is a pointer type
    // TODO : this implementation makes me feel sick (list of pointers may fail -> endswith __ptr64?)
    return ext::string{type().name()}.contains("* __ptr64");
}


auto ext::any::is_numeric() const -> bool
{
    // return is the internal type is numeric
    try
    {
        return any_cast<int>(*this);
    }
    catch (const std::bad_any_cast&)
    {
        return false;
    }
}


template <typename T>
auto ext::any::emplace(T&& element) -> any&
{
    // emplace an object of type T as the internal object being wrapped by ext::any
    m_value.template emplace<T>(element);
    return *this;
}


template <typename T>
auto ext::any::emplace() -> any&
{
    // emplace an empty object of type T (ie assign the type as T but .empty() will return true)
    m_value.template emplace<T>();
    return *this;
}


template <typename T>
auto ext::any::to() const -> T
{
    // cast the internal object wrapped to any type T
    return any_cast<T>(m_value);
}


ext::any::operator bool() const
{
    // the conversion will only return true if there is an internal object wrapped TODO : false / nullptr will return true -> special case?
    return not empty();
}


template <class T>
auto any_cast(const ext::any& value) -> T
{
    // if the value is empty or a void, then return a nullptr for pointer types, otherwise a new stack object of type T
    // ie 0 for an int or "" for a string etc
    if (value.empty() or value.type() == typeid(void))
        return value.contains_pointer() ? nullptr : T();

    // otherwise, return the internal object unwrapped, as type T
    return std::any_cast<T>(value);
}


#endif //SBROWSER_ANY_HPP
