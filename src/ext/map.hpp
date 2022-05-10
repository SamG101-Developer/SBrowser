#pragma once
#ifndef SBROWSER_MAP_HPP
#define SBROWSER_MAP_HPP

namespace ext {template <typename K, typename V> class map;}

#include <map>
#include <ranges>

#include <ext/any.hpp>
#include <ext/exception.hpp>
#include <ext/iterable.hpp>
#include <ext/string.hpp>

namespace ext {using string_any_map_t    = map<string, any   >;}
namespace ext {using string_string_map_t = map<string, string>;}


template <typename K, typename V>
class ext::map final : public ext::iterable<V, std::map<K, V>>
{
public aliases:
    using pair_t = std::pair<K, V>;

public constructors:
    map() = default;
    map(const map&) = default;
    map(map&&) noexcept = default;
    auto operator=(const map&) -> map& = default;
    auto operator=(map&&) noexcept -> map& = default;
    ~map() override = default;

    explicit map(const std::initializer_list<pair_t>&& that);
    auto operator=(std::initializer_list<pair_t>&& that) -> map&;

public js_methods:
    auto insert(const K& key, const V& value) -> map&;
    auto at(const K& key) const -> const V&;
    auto at(K& key) -> V&;
    auto has_key(const K& key) const -> bool;

    auto keys() -> ext::vector<K>;
    auto values() -> ext::vector<V>;

    template <typename U> auto cast_all() -> map<K, U>;
    template <typename U> auto cast_all() -> map<K, U> requires (std::is_same_v<V, ext::any>);

public operators:
    auto operator==(const map& that) -> bool;
};


template <typename K, typename V>
ext::map<K, V>::map(
        const std::initializer_list<pair_t>&& that)
{
    // create the internal container from the initialization list
    this->m_iterable = that;
}


template <typename K, typename V>
auto ext::map<K, V>::operator=(
        std::initializer_list<pair_t>&& that)
        -> map&
{
    // create the internal container from the initialization list
    this->m_iterable = std::move(that);
}


template <typename K, typename V>
auto ext::map<K, V>::insert(
        const K& key,
        const V& value)
        -> map&
{
    // insert the key-value pair, and return the reference to the map
    this->m_iterable.emplace(key, value);
    return *this;
}


template <typename K, typename V>
auto ext::map<K, V>::at(
        const K& key) const
        -> const V&
{
    if (not has_key(key))
        throw std::invalid_argument{"Map doesn't contain the key " + key};

    // return the item in the middle of the map
    return this->m_iterable.at(key);
}


template <typename K, typename V>
auto ext::map<K, V>::at(
        K& key)
        -> V&
{
    // return the item in the middle of the map
    return this->m_iterable.at(key);
}


template <typename K, typename V>
auto ext::map<K, V>::has_key(
        const K& key) const
        -> bool
{
    // check if the iterable contains a key by comparing its iterator location to the end iterator
    return this->m_iterable.find(key) != this->m_iterable.end();
}


template <typename K, typename V>
auto ext::map<K, V>::keys()
        -> ext::vector<K>
{
    // return a vector of the keys
    auto keys = std::views::keys(this->m_iterable);
    return ext::vector<K>{keys.begin(), keys.end()};
}


template <typename K, typename V>
auto ext::map<K, V>::values()
        -> ext::vector<V>
{
    // return a vector of the values
    auto values = std::views::values(this->m_iterable);
    return ext::vector<V>{values.begin(), values.end()};
}


template <typename K, typename V>
template <typename U>
auto ext::map<K, V>::cast_all()
        -> map<K, U>
{
    // throw an error as this non-specialized method cannot call objects that aren't of a callable type V
    throw ext::type_error{"Type V must be the 'ext::any' type"};
}


template <typename K, typename V>
template <typename U>
auto ext::map<K, V>::cast_all()
        -> ext::map<K, U> requires (std::is_same_v<V, ext::any>)
{
    // create an empty copy of the map, ext::any should automatically cast from T -> V
    map<K, U> copy {*this};

    // return the cast map
    return copy;
}


template <typename K, typename V>
auto ext::map<K, V>::operator==(
        const map& that)
        -> bool
{
    // guard to check that the lengths match
    if (this->length() != that.length())
        return false;

    // inequality check by comparing the items in the two iterables
    return std::ranges::all_of(this->begin(), this->end(), [this, that](const K& key) {return that.has_key(key) and at(key) == that.at(key);});
}



#endif //SBROWSER_MAP_HPP
