#ifndef SBROWSER_ANY_HPP
#define SBROWSER_ANY_HPP

#include <any>

#include <ext/decorators.hpp>
#include <ext/string.hpp>


//inline bool operator< (const std::any& first, const std::any& second) {return &first <  &second;}
//inline bool operator> (const std::any& first, const std::any& second) {return &first >  &second;}
//inline bool operator==(const std::any& first, const std::any& second) {return &first == &second;}


namespace ext {
    class any;
    template <class T> T any_cast(ext::any value);
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
    ext::any& operator=(const std::any& other) {m_value = other; return *this;}
    ext::any& operator=(std::any&& other) noexcept {m_value = other; return *this;}

public: // TODO : annotation
    operator std::any() const {
        return m_value;
    }

    template <typename T>
    explicit operator T() const requires (not std::is_same_v<T, std::any>) {
        return ext::any_cast<T>(m_value);
    }

public: methods
    const type_info& type() const {
        return m_value.type();
    }

    bool empty() const {
        return not m_value.has_value();
    }

    bool contains_pointer() const {
        return ext::string{type().name()}.contains(*"* __ptr64");
    }

    template <typename T>
    void emplace(T&& element) {
        m_value.template emplace<T>(element);
    }

private: internal_properties
    std::any m_value;
};


template <class T>
T ext::any_cast(ext::any value) {
    if (value.empty() or value.type() == typeid(value))
        return value.contains_pointer() ? nullptr : T();
    try {
        return std::any_cast<T>(value);
    } catch(...) {
        return std::any_cast<T>(std::any());
    }
}


#endif //SBROWSER_ANY_HPP
