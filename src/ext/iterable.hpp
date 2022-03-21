#pragma once
#ifndef SBROWSER_ITERABLE_HPP
#define SBROWSER_ITERABLE_HPP

#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <numeric>
#include <stdexcept>

#include <ext/cpp_keywords.hpp>
#include <ext/decorators.hpp>

namespace ext {template <typename T, typename C> class iterable;}


template <typename T, typename C>
class ext::iterable
{
public aliases:
    using iterator = typename C::iterator;
    using const_iterator = typename C::const_iterator;

public constructors:
    iterable() = default;
    iterable(const iterable<T, C>&) = default;
    iterable(iterable<T, C>&&) noexcept = default;
    iterable<T, C>& operator=(const iterable<T, C>&) = default;
    iterable<T, C>& operator=(iterable<T, C>&&) noexcept = default;

    virtual ~iterable() = default;

    auto front() noexcept(false) -> T&;
    auto back() noexcept(false) -> T&;
    auto at(size_t i) const noexcept(false) -> const T&;
    auto at(size_t i) noexcept(false) -> T&;
    auto at(const_iterator i) noexcept(false) -> T&;

    auto begin() -> iterator;
    auto begin() const -> const_iterator ;
    auto end() -> iterator;
    auto end() const -> const_iterator;

    auto empty() const noexcept -> bool;
    auto length() const noexcept -> size_t;

    virtual auto clear() -> iterable<T, C>&;
    template <class F> auto remove_if(const F& function, bool all = false) -> iterable<T, C>&;
    template <class F> auto replace_if(const F& function, const T& new_item, bool all = false) -> iterable<T, C>&;
    auto remove(const T& item, bool all = false) -> iterable<T, C>&;
    auto replace(const T& old_item, const T& new_item, bool all = false) -> iterable<T, C>&;
    auto reverse() -> iterable<T, C>&;
    auto sort() -> iterable<T, C>&;
    auto clean() -> iterable<T, C>& requires std::is_pointer_v<T>;

    auto reversed() const -> iterable<T, C>;
    auto sorted() const -> iterable<T, C>;

    auto find(const T& object, size_t offset = 0) const -> size_t;
    auto contains(const T& item) const -> bool;
    auto print() const -> void;

public operators:
    virtual auto operator!() const -> bool;
    auto operator==(const iterable<T, C>& o) const -> bool;

protected internal_properties:
    C m_iterable;
};


template <typename T, typename C>
auto ext::iterable<T, C>::front() noexcept(false) -> T&
{
    // throws error if accessing the front of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access front of an empty iterable"};

    // return the item at the front of the iterable
    return m_iterable.front();
}


template <typename T, typename C>
auto ext::iterable<T, C>::back() noexcept(false) -> T&
{
    // throws error if accessing the back of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access back of an empty iterable"};

    // return the item at the back of the iterable
    return m_iterable.back();
}


template<typename T, typename C>
auto ext::iterable<T, C>::at(size_t i) const noexcept(false) -> const T&
{
    // throws error if accessing the middle of an empty iterable
    if (empty())
        throw std::out_of_range("Cannot access nth item of an empty iterable");

    // return the item in the middle of the iterable
    return m_iterable.at(i);
}


template <typename T, typename C>
auto ext::iterable<T, C>::at(const size_t i) noexcept(false) -> T&
{
    // throws error if accessing the middle of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access nth item of an empty iterable"};

    // return the item in the middle of the iterable
    return m_iterable.at(i);
}


template <typename T, typename C>
auto ext::iterable<T, C>::at(const_iterator i) noexcept(false) -> T&
{
    // throws error if accessing the middle of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access nth-iterator item of an empty iterable"};

    // return the item in the middle of the iterable
    return m_iterable.at(*i);
}


template <typename T, typename C>
auto ext::iterable<T, C>::begin() -> ext::iterable<T, C>::iterator
{
    // return the begin iterator for the iterable
    return m_iterable.begin();
}


template <typename T, typename C>
auto ext::iterable<T, C>::begin() const -> ext::iterable<T, C>::const_iterator
{
    // return the const begin iterator for the iterable
    return m_iterable.begin();
}


template <typename T, typename C>
auto ext::iterable<T, C>::end() -> ext::iterable<T, C>::iterator
{
    // return the end iterator for the iterable
    return m_iterable.end();
}


template <typename T, typename C>
auto ext::iterable<T, C>::end() const -> ext::iterable<T, C>::const_iterator
{
    // return the const end iterator for the iterable
    return m_iterable.end();
}


template <typename T, typename C>
auto ext::iterable<T, C>::empty() const noexcept -> bool
{
    // return if the iterable is empty or not
    return m_iterable.empty();
}


template <typename T, typename C>
auto ext::iterable<T, C>::length() const noexcept -> size_t
{
    // return the size of the iterable
    return m_iterable.size();
}


template <typename T, typename C>
auto ext::iterable<T, C>::clear() -> ext::iterable<T, C>&
{
    // clear the iterable, and return the reference to it
    m_iterable.clear();
    return *this;
}


template <typename T, typename C>
template <class F>
auto ext::iterable<T, C>::remove_if(
        const F& function,
        const bool all)
        -> ext::iterable<T, C>&
{
    // continue to loop while the function matches items in the iterable
    while (std::find_if(begin(), end(), function) != end())
    {
        // remove the item, and continue looping if looking for all matches
        std::remove_if(begin(), end(), function);
        if (not all) break;
    }

    // return the point to the iterable
    return *this;
}


template <typename T, typename C>
template <class F>
auto ext::iterable<T, C>::replace_if(
        const F& function,
        const T& new_item,
        const bool all)
        -> ext::iterable<T, C>&
{
    // continue to loop while the function matches items in the iterable
    while (std::find_if(begin(), end(), function) != end())
    {
        // replace the item, and continue looping if looking for all matches
        std::replace_if(begin(), end(), function, new_item);
        if (not all) break;
    }

    // return the reference to the iterable
    return *this;
}


template <typename T, typename C>
auto ext::iterable<T, C>::remove(
        const T& item,
        const bool all)
        -> ext::iterable<T, C>&
{
    // continue to loop while the item is in the iterable
    while (contains(item))
    {
        // remove the item, and continue looping if looking for all matches
        auto _ = std::remove(begin(), end(), item);
        if (not all) break;
    }

    // return the reference to the iterable
    return *this;
}


template <typename T, typename C>
auto ext::iterable<T, C>::replace(
        const T& old_item,
        const T& new_item,
        const bool all)
        -> ext::iterable<T, C>&
{
    // continue to loop while the item is in the iterable
    while (contains(old_item))
    {
        // replace the item and continue looping if looking for all matches
        std::replace(begin(), end(), old_item, new_item);
        if (not all) break;
    }

    // return the reference to the iterable
    return *this;
}


template <typename T, typename C>
auto ext::iterable<T, C>::reverse() -> ext::iterable<T, C>&
{
    // reverse the iterable, and return the reference to it
    std::ranges::reverse(m_iterable.begin(), m_iterable.end());
    return *this;
}


template <typename T, typename C>
auto ext::iterable<T, C>::sort() -> ext::iterable<T, C>&
{
    // sort the iterable, and return the reference to it
    std::ranges::sort(m_iterable.begin(), m_iterable.end());
    return *this;
}


template <typename T, typename C>
auto ext::iterable<T, C>::clean() -> ext::iterable<T, C>& requires std::is_pointer_v<T>
{
    // remove all the nullptr from the iterable, and return the reference to it
    if constexpr(std::is_pointer_v<T>)
        remove(nullptr, true);

    return *this;
}


template <typename T, typename C>
auto ext::iterable<T, C>::reversed() const -> ext::iterable<T, C>
{
    // reverse a duplicate iterable, and return the reference to it
    return iterable<T, C>{*this}.reverse();
}


template <typename T, typename C>
auto ext::iterable<T, C>::sorted() const -> ext::iterable<T, C>
{
    // sort a duplicate iterable, and return the reference to it
    return iterable<T, C>{*this}.sort();
}


template <typename T, typename C>
auto ext::iterable<T, C>::find(const T& object, const size_t offset) const -> size_t
{
    // return the index of an item in the iterable by comparing iterator positions
    return std::distance(begin(), std::find(begin() + offset, end(), object));
}


template <typename T, typename C>
auto ext::iterable<T, C>::contains(const T& item) const -> bool
{
    // check if the iterable contains an item by comparing its iterator location to the end iterator
    return begin() + find(item) != end();
}


template <typename T, typename C>
auto ext::iterable<T, C>::print() const -> void
{
    // serialize the iterable by output the list as a string - TODO: MOVE TO OPERATOR <<
    std::cout << std::copy(begin(), end(), std::ostream_iterator<const char*>(std::cout, ", ")) << std::endl;
}


template <typename T, typename C>
auto ext::iterable<T, C>::operator!() const -> bool
{
    // the iterable evaluates to false if the list is empty (inverse operator ie -> true)
    return empty();
}


template <typename T, typename C>
auto ext::iterable<T, C>::operator==(const iterable<T, C>& o) const -> bool
{
    // guard to check that the lengths match
    if (length() != o.length())
        return false;

    // create a range of indexes the length of the iterable
    std::vector<size_t> range;
    range.reserve(length());
    std::iota(range.begin(), range.end(), 0);

    // equality check by comparing the items in the two iterables
    return std::all_of(range.begin(), range.end(), [this, o](const size_t i) {return at(i) == o.at(i);});
}



template class ext::iterable<int, std::vector<int>>;
template class ext::iterable<int, std::deque<int>>;



#endif //SBROWSER_ITERABLE_HPP
