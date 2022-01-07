#pragma once
#ifndef SBROWSER_MAP_HPP
#define SBROWSER_MAP_HPP

#include <map>

#include <ext/types/any.hpp>
#include <ext/iterables/iterable.hpp>

namespace ext {
    template <class K, class V> class map;
    template <class K, class V> using cmap = const map<K, V>;

    using string_any_map = map<string, any>;
    using cstring_any_map = const map<string, any>;
}


template <class K, class V>
class ext::map : public ext::iterable<V, std::map<K, V>> {
public: constructors
    map() = default;
    map(const map<K, V>&) = default;
    map(map<K, V>&&) noexcept = default;
    map<K, V>& operator=(const map<K, V>&) = default;
    map<K, V>& operator=(map<K, V>&&) noexcept = default;

    explicit map(std::initializer_list<std::pair<K, V>> other) {
        for (const auto& [key, value]: other) insert(key, value);
    }

public: methods
    // element access
    inline ext::map<K, V>& insert(const K& key, const V& value) {
        this->m_iterable.emplace(key, value);
        return *this;
    }

    inline const V& at(const K& key) const {
        return this->m_iterable.at(key);
    }

    inline V& at(const K& key) {
        return this->m_iterable.at(key);
    }

    inline bool has_key(const K& key) {
        return this->m_iterable.find(key) != this->m_iterable.end();
    }

    // modifiers
    template <typename U> inline map<K, U> cast_all() requires std::is_same_v<V, std::any> {
        map<K, U> copy;
        for (const auto& [key, value]: *this)
            copy.at(key) = value;
        return copy;
    }

public: operators
    inline bool operator==(ext::cmap<K, V>& other) {
        for (const auto& [key, value]: this->m_iterable) {
            if (not (has_key(key) and at(key) == value)) return false;
        }
        return true;
    }
};


#endif //SBROWSER_MAP_HPP
