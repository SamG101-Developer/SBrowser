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
    map(std::initializer_list<std::pair<K, V>>& o);
    map<K, V>& operator=(const map<K, V>&) = default;
    map<K, V>& operator=(map<K, V>&&) noexcept = default;

public: methods
    // element access
    ext::map<K, V>& insert(const K& key, const V& value);
    V& at(const K& key) const;
    bool has_key(const K& key);

    // modifiers
    template <typename U> map<K, U> cast_all() requires std::is_same_v<V, ext::any>;

public: operators
    bool operator==(ext::cmap<K, V>& o);
};


template <typename K, typename V>
ext::map<K, V>::map(std::initializer_list<std::pair<K, V>>& o)
{
    // add each key-value pair sequentially to the map
    for (const auto& [key, value]: o) insert(key, value);
}


template <typename K, typename V>
inline ext::map<K, V>& ext::map<K, V>::insert(const K& key, const V& value)
{
    // insert the key-value pair, and return the reference to the map
    this->m_iterable.emplace(key, value);
    return *this;
}


template <typename K, typename V>
inline V& ext::map<K, V>::at(const K& key) const
{
    // return the item in the middle of the map
    return this->m_iterable.at(key);
}


template <typename K, typename V>
inline bool ext::map<K, V>::has_key(const K& key)
{
    // check if the iterable contains a key by comparing its iterator location to the end iterator
    return this->m_iterable.find(key) != this->m_iterable.end();
}


template <typename K, typename V>
template <typename U>
inline ext::map<K, U> ext::map<K, V>::cast_all() requires std::is_same_v<V, ext::any>
{
    // create an empty copy of the map
    map<K, U> copy;

    // add each element into the copy, and the ext::any_cast will automatically convert the value type
    for (const auto& [key, value]: *this)
        copy.at(key) = value;

    // return the casted map
    return copy;
}


template <typename K, typename V>
inline bool ext::map<K, V>::operator==(ext::cmap<K, V>& o)
{
    // guard to check that the lengths match
    if (this->length() != o.length())
        return false;

    // inequality check by comparing the items in the two iterables
    return std::all_of(this->begin(), this->end(), [this, o](const K& key) {return at(key) == o.at(key);});
}



#endif //SBROWSER_MAP_HPP
