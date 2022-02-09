#pragma once
#ifndef SBROWSER_MAP_HPP
#define SBROWSER_MAP_HPP

#include <map>

#include <ext/types/any.hpp>
#include <ext/iterables/iterable.hpp>

namespace ext {
    template <typename K, typename V> class map;
    template <typename K, typename V> using cmap = const map<K, V>;

    using string_any_map = map<string, any>;
    using cstring_any_map = const map<string, any>;
}


template <typename K, typename V>
class ext::map : public ext::iterable<V, std::map<K, V>> {
public: constructors
    map() = default;
    map(const map<K, V>&) = default;
    map(map<K, V>&&) noexcept = default;
    map<K, V>& operator=(const map<K, V>&) = default;
    map<K, V>& operator=(map<K, V>&&) noexcept = default;
    map(std::initializer_list<std::pair<K, V>>& other);

public: methods
    // element access
    ext::map<K, V>& insert(const K& key, const V& value);
    const V& at(const K& key) const;
    bool has_key(const K& key);

    // modifiers
    template <typename U> map<K, U> cast_all() requires std::is_same_v<V, std::any>;

public: operators
    bool operator==(ext::cmap<K, V>& other);
};


template <typename K, typename V>
ext::map<K, V>::map(std::initializer_list<std::pair<K, V>>& other) {
    for (const auto& [key, value]: other) insert(key, value);
}

template <typename K, typename V>
inline ext::map<K, V>& ext::map<K, V>::insert(const K& key, const V& value) {
    this->m_iterable.emplace(key, value);
    return *this;
}

template <typename K, typename V>
inline const V& ext::map<K, V>::at(const K& key) const {
    return this->m_iterable.at(key);
}

template <typename K, typename V>
inline bool ext::map<K, V>::has_key(const K& key) {
    return this->m_iterable.find(key) != this->m_iterable.end();
}

template <typename K, typename V>
template <typename U>
inline ext::map<K, U> ext::map<K, V>::cast_all() requires std::is_same_v<V, std::any> {
    map<K, U> copy;
    for (const auto& [key, value]: *this)
        copy.at(key) = value;
    return copy;
}

template <typename K, typename V>
inline bool ext::map<K, V>::operator==(ext::cmap<K, V>& other) {
    for (const auto& [key, value]: this->m_iterable) {
        if (not (has_key(key) and at(key) == value)) return false;
    }
    
    return true;
}



#endif //SBROWSER_MAP_HPP
