#ifndef SBROWSER_ANY_HPP
#define SBROWSER_ANY_HPP

#include <any>

#include <ext/macros/decorators.hpp>
#include <ext/iterables/string.hpp>


//inline bool operator< (const std::any& first, const std::any& second) {return &first <  &second;}
//inline bool operator> (const std::any& first, const std::any& second) {return &first >  &second;}
//inline bool operator==(const std::any& first, const std::any& second) {return &first == &second;}


namespace ext {
    class any;
    using cany = const any;
}
namespace {template <class T> T any_cast(ext::any value);}


class ext::any {
public: constructors
    any() = default;
    any(const ext::any&) = default;
    any(ext::any&&) noexcept = default;
    ext::any& operator=(const ext::any&) = default;
    ext::any& operator=(ext::any&&) noexcept = default;

    template <typename T> any(const T& other) {m_value = other;};
    template <typename T> any(T&& other) noexcept {m_value = other;};
    template <typename T> any& operator=(const T& other) {m_value = other; return *this;}
    template <typename T> any& operator=(T&& other) noexcept {m_value = other; return *this;}

public: methods
    func type() const -> const type_info&;
    func empty() const -> bool;
    func contains_pointer() const -> bool;
    func is_numeric() const -> bool;
    template <typename T> func emplace() -> void;
    template <typename T> func emplace(T&& element) -> void;
    template <typename T> func to() const -> T;

public: operators
    operator bool() const;

private: internal_properties
    std::any m_value;
};


bool operator==(ext::cany& first, ext::cany& second)
{
    // equality check by comparing the addresses of the two objects
    return &first == &second;
}


const type_info& ext::any::type() const
{
    // return the type of the internal object wrapped with the ext::any
    return m_value.type();
}


bool ext::any::empty() const
{
    // return if the ext::any object is empty ie there is no internal object wrapped
    return not m_value.has_value();
}


bool ext::any::contains_pointer() const
{
    // return if the internal object wrapped is a pointer type TODO : this implementation makes me feel sick
    return ext::string{type().name()}.contains("* __ptr64");
}


func ext::any::is_numeric() const -> bool
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
void ext::any::emplace(T&& element)
{
    // emplace an object of type T as the internal object being wrapped by ext::any
    m_value.template emplace<T>(element);
}


template <typename T>
void ext::any::emplace()
{
    // emplace an empty object of type T (ie assign the type as T but .empty() will return true)
    m_value.template emplace<T>();
}


template <typename T>
T ext::any::to() const
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
T any_cast(ext::cany& value)
{
    // if the value is empty or a void, then return a nullptr for pointer types, otherwise a new stack object of type T
    if (value.empty() or value.type() == typeid(void))
        return value.contains_pointer() ? nullptr : T();

    // otherwise, return the internal object unwrapped, as type T
    return std::any_cast<T>(value);
}


#endif //SBROWSER_ANY_HPP
