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
    const type_info& type() const;
    bool empty() const;
    bool contains_pointer() const;
    template <typename T> void emplace();
    template <typename T> void emplace(T&& element);
    template <typename T> T to() const;

public: operators
    operator bool() const;

private: internal_properties
    std::any m_value;
};


bool operator==(ext::cany& first, ext::cany& second) {
    return &first == &second;
}


const type_info& ext::any::type() const {
    return m_value.type();
}

bool ext::any::empty() const {
    return not m_value.has_value();
}

bool ext::any::contains_pointer() const {
    return ext::string{type().name()}.contains("* __ptr64");
}

template <typename T>
void ext::any::emplace(T&& element) {
    m_value.template emplace<T>(element);
}

template <typename T>
void ext::any::emplace() {
    m_value.template emplace<T>();
}

template <typename T>
T ext::any::to() const {
    return any_cast<T>(m_value);
}

ext::any::operator bool() const {
    return not empty();
}


template <class T>
T any_cast(ext::cany& value) {
    if (value.empty() or value.type() == typeid(void))
        return value.contains_pointer() ? nullptr : T();
    return std::any_cast<T>(value);
}


#endif //SBROWSER_ANY_HPP
