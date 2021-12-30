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

    template <typename T> any(const T& other) : m_value(other) {};
    template <typename T> any(T&& other) noexcept : m_value(other) {};
    template <typename T> ext::any& operator=(const T& other) {m_value = other; return *this;}
    template <typename T> ext::any& operator=(T&& other) noexcept {m_value = other; return *this;}

public: // TODO : annotation
    operator std::any() const {
        return m_value;
    }

    template <typename T>
    explicit operator T() const requires (not std::is_same_v<T, std::any>) {
        return any_cast<T>(m_value);
    }

public: methods
    const type_info& type() const {
        return m_value.type();
    }

    bool empty() const {
        return not m_value.has_value();
    }

    bool contains_pointer() const {
        return ext::string{type().name()}.contains("* __ptr64");
    }

    bool is_numeric() const {
        return type() == typeid(unsigned char) or type() == typeid(unsigned short) or type() == typeid(unsigned int)
                or type() == typeid(unsigned long) or type() == typeid(unsigned long long) or type() == typeid(char)
                or type() == typeid(short) or type() == typeid(int) or type() == typeid(long) or type() == typeid(long long)
                or type() == typeid(float) or type() == typeid(double) or type() == typeid(long double);
    }

    template <typename T>
    void emplace(T&& element) {
        m_value.template emplace<T>(element);
    }

    template <typename T>
    void emplace() {
        m_value.template emplace<T>();
    }

private: internal_properties
    std::any m_value;
};


template <class T>
T any_cast(ext::any value) {
    if (value.empty() or value.type() == typeid(void))
        return value.contains_pointer() ? nullptr : T();
    return value;
}


#endif //SBROWSER_ANY_HPP
