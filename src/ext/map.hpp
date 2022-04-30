#pragma once
#ifndef SBROWSER_MAP_HPP
#define SBROWSER_MAP_HPP

#include <map>
#include <ranges>

#include <ext/any.hpp>
#include <ext/iterable.hpp>
#include <ext/string.hpp>


namespace ext {template <typename K, typename V> class map;}
namespace ext {template <typename K, typename V> using cmap = const map<K, V>;}
namespace ext {using string_any_map = map<string, any>;}
namespace ext {using string_string_map = map<string, string>;}


template <typename K, typename V>
class ext::map final: public ext::iterable<V, std::map<K, V>>
{
public aliases:
    using pair_t = std::pair<K, V>;

public constructors:
    map() = default;
    map(const map<K, V>&) = default;
    map(map<K, V>&&) noexcept = default;
    map<K, V>& operator=(const map<K, V>&) = default;
    map<K, V>& operator=(map<K, V>&&) noexcept = default;

    map(const std::initializer_list<pair_t>&& o);
    map<K, V>& operator=(std::initializer_list<pair_t>&& o);

public js_methods:
    auto insert(const K& key, const V& value) -> ext::map<K, V>&;
    auto at(const K& key) const -> const V&;
    auto at(K& key) -> V&;
    auto has_key(const K& key) const -> bool;

    auto keys() -> ext::vector<K>;
    auto values() -> ext::vector<V>;

    template <typename U> auto cast_all() -> map<K, U> requires std::is_same_v<V, ext::any>;

public operators:
    auto operator==(ext::cmap<K, V>& o) -> bool;
};


template <typename K, typename V>
ext::map<K, V>& ext::map<K, V>::operator=(std::initializer_list<pair_t>&& o)
{
    // append the moved args (std::pair<K, V>)
    for (const pair_t& pair: o)
        this->insert(pair.first, pair.second);
}


template <typename K, typename V>
auto ext::map<K, V>::insert(const K& key, const V& value) -> ext::map<K, V>&
{
    // insert the key-value pair, and return the reference to the map
    this->m_iterable.emplace(key, value);
    return *this;
}


template <typename K, typename V>
auto ext::map<K, V>::at(const K& key) const -> const V&
{
    // return the item in the middle of the map
    return this->m_iterable.at(key);
}


template <typename K, typename V>
auto ext::map<K, V>::at(K& key) -> V&
{
    // return the item in the middle of the map
    return this->m_iterable.at(key);
}


template <typename K, typename V>
auto ext::map<K, V>::has_key(const K& key) const -> bool
{
    // check if the iterable contains a key by comparing its iterator location to the end iterator
    return this->m_iterable.find(key) != this->m_iterable.end();
}


template <typename K, typename V>
auto ext::map<K, V>::keys() -> ext::vector<K>
{
    // return a vector of the keys
    auto keys = std::views::keys(this->m_iterable);
    return ext::vector<K>{keys.begin(), keys.end()};
}


template <typename K, typename V>
auto ext::map<K, V>::values() -> ext::vector<V>
{
    // return a vector of the values
    auto values = std::views::values(this->m_iterable);
    return ext::vector<V>{values.begin(), values.end()};
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
auto ext::map<K, V>::operator==(ext::cmap<K, V>& o) -> bool
{
    // guard to check that the lengths match
    if (this->length() != o.length())
        return false;

    // inequality check by comparing the items in the two iterables
    return std::ranges::all_of(this->begin(), this->end(), [this, o](const K& key) {return at(key) == o.at(key);});
}



#endif //SBROWSER_MAP_HPP
