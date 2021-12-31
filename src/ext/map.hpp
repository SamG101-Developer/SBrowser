#pragma once
#ifndef SBROWSER_MAP_HPP
#define SBROWSER_MAP_HPP

#include <map>

#include <ext/any.hpp>
#include <ext/iterable.hpp>

namespace ext {
    template <class K, class V> class map;
    using string_any_map  = map<string, ext::any>;
    using cstring_any_map = const ext::string_any_map;
}


template <class K, class V>
class ext::map : public ext::iterable<V, std::map<K, V>> {
public:
    map() = default;
    map(const map<K, V>&) = default;
    map(map<K, V>&&) noexcept = default;
    map<K, V>& operator=(const map<K, V>&) = default;
    map<K, V>& operator=(map<K, V>&&) noexcept = default;

    explicit map(std::initializer_list<std::pair<K, V>> other) {for (const auto& [key, value]: other) insert(key, value);}

    inline ext::map<K, V>& insert(const K& key, const V& value) {
        this->m_iterable.emplace(key, value);
    }

    template <typename U> inline map<K, U> cast_all() requires std::is_same_v<V, std::any> {
        map<K, U> copy;
        for (const auto& [key, value]: *this)
            copy.at(key) = value;
        return copy;
    }

    inline V& at(const K& key) const {
        return this->m_iterable.at(key);
    }

    inline bool has_key(const K& key) {
        return this->m_iterable.find(key) != this->m_iterable.end();
    }
};


#endif //SBROWSER_MAP_HPP
