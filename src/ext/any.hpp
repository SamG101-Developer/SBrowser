#ifndef SBROWSER_ANY_HPP
#define SBROWSER_ANY_HPP

#include <any>

#include <ext/decorators.hpp>


//inline bool operator< (const std::any& first, const std::any& second) {return &first <  &second;}
//inline bool operator> (const std::any& first, const std::any& second) {return &first >  &second;}
//inline bool operator==(const std::any& first, const std::any& second) {return &first == &second;}


namespace ext {
    template <class T> T any_cast(std::any value);
    class any;
}


template <class T>
T ext::any_cast(std::any value) {
    if (not value.has_value() or value.type() == typeid(value))
        return T(); // std::is_pointer_v<T> ? nullptr : T(); -> if constexpr () {}
    try {
        return std::any_cast<T>(value);
    } catch(...) {
        return std::any_cast<T>(std::any());
    }
}


class ext::any {
public: constructors
    any() = default;
    any(const ext::any&) = default;
    any(ext::any&&) noexcept = default;
    ext::any& operator=(const ext::any&) = default;
    ext::any& operator=(ext::any&&) noexcept = default;

    explicit any(const std::any& other) : m_value(other) {};
    explicit any(std::any&& other) noexcept : m_value(other) {};

    operator std::any() const {return m_value;}

    template <typename T>
    explicit operator T() const requires (not std::is_same_v<T, std::any>) {
        return ext::any_cast<T>(m_value);
    }

public: methods
    const type_info& type() const {return m_value.type();}
    const bool empty() const {return not m_value.has_value();}

private: internal_properties
    std::any m_value;
};


#endif //SBROWSER_ANY_HPP
