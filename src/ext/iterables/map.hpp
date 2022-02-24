#pragma once
#ifndef SBROWSER_MAP_HPP
#define SBROWSER_MAP_HPP

#include <map>

#include <ext/types/any.hpp>
#include <ext/iterables/iterable.hpp>

namespace ext
{
    template <typename K, typename V> class map;
    template <typename K, typename V> using cmap = const map<K, V>;

    using string_any_map = map<string, any>;
    using cstring_any_map = const map<string, any>;
}


template <typename K, typename V>
class ext::map : public ext::iterable<V, std::map<K, V>>
{
public: constructors
    map() = default;
    map(const map<K, V>&) = default;
    map(map<K, V>&&) noexcept = default;
    map(const std::initializer_list<std::pair<K, V>>& o);
    map<K, V>& operator=(const map<K, V>&) = default;
    map<K, V>& operator=(map<K, V>&&) noexcept = default;

public: methods
    auto insert(const K& key, const V& value) -> ext::map<K, V>&;
    auto at(const K& key) const -> V&;
    auto has_key(const K& key) -> bool;

    template <typename U> auto cast_all() -> map<K, U> requires std::is_same_v<V, ext::any>;
    template <typename F> auto filter_keys(F&& function) -> ext::vector<K>;
    template <typename F> auto filter_values(F&& function) -> ext::vector<V>;

public: operators
    auto operator==(ext::cmap<K, V>& o) -> bool;
};


template <typename K, typename V>
ext::map<K, V>::map(const std::initializer_list<std::pair<K, V>>& o)
{
    // add each key-value pair sequentially to the map
    for (const auto& [key, value]: o) insert(key, value);
}


template <typename K, typename V>
auto ext::map<K, V>::insert(const K& key, const V& value) -> ext::map<K, V>&
{
    // insert the key-value pair, and return the reference to the map
    this->m_iterable.emplace(key, value);
    return *this;
}


template <typename K, typename V>
auto ext::map<K, V>::at(const K& key) const -> V&
{
    // return the item in the middle of the map
    return this->m_iterable.at(key);
}


template <typename K, typename V>
auto ext::map<K, V>::has_key(const K& key) -> bool
{
    // check if the iterable contains a key by comparing its iterator location to the end iterator
    return this->m_iterable.find(key) != this->m_iterable.end();
}


template <typename K, typename V>
template <typename U>
auto ext::map<K, V>::cast_all() -> ext::map<K, U> requires std::is_same_v<V, ext::any>
{
    // create an empty copy of the map
    map<K, U> copy;

    // add each element into the copy, and the ext::any_cast will automatically convert the value type
    for (const auto& [key, value]: *this)
        copy.at(key) = value;

    // return the cast map
    return copy;
}


template <typename K, typename V>
template <typename F>
auto ext::map<K, V>::filter_keys(F&& function) -> ext::vector<K>
{
    // create the empty keys list
    ext::vector<K> filtered_keys;

    // filter the keys into a list
    for (auto [key, val]: this->m_iterable)
    {
        if (function(key)) filtered_keys.append(key);
    }

    // return the filtered keys
    return filtered_keys;
}


template <typename K, typename V>
template <typename F>
auto ext::map<K, V>::filter_values(F&& function) -> ext::vector<V>
{
    // create the empty values list
    ext::vector<V> filtered_values;

    // filter the values into a list
    for (auto [key, val]: this->m_iterable)
    {
        if (function) filtered_values.append(val);
    }

    // return the filtered keys
    return filtered_values;
}


template <typename K, typename V>
auto ext::map<K, V>::operator==(ext::cmap<K, V>& o) -> bool
{
    // guard to check that the lengths match
    if (this->length() != o.length())
        return false;

    // inequality check by comparing the items in the two iterables
    return std::all_of(this->begin(), this->end(), [this, o](const K& key) {return at(key) == o.at(key);});
}



#endif //SBROWSER_MAP_HPP
