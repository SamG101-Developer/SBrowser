#pragma once
#ifndef SBROWSER_ITERABLE_HPP
#define SBROWSER_ITERABLE_HPP

#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdexcept>

#include <ext/macros/cpp_keywords.hpp>
#include <ext/macros/decorators.hpp>

namespace ext {template <typename T, typename C> class iterable;}

template <typename T, typename C>
class ext::iterable {
aliases
    using iterator = typename C::iterator;
    using const_iterator = typename C::const_iterator;

public: constructors
    iterable() = default;
    iterable(const iterable<T, C>&) = default;
    iterable(iterable<T, C>&&) noexcept = default;
    iterable<T, C>& operator=(const iterable<T, C>&) = default;
    iterable<T, C>& operator=(iterable<T, C>&&) noexcept = default;

    virtual ~iterable() = default;

    // element access
    function front() const noexcept(false) -> T&;
    function back() const noexcept(false) -> T&;
    function at(const size_t i) const noexcept(false) -> T&;
    function at(const_iterator i) const noexcept(false) -> T&;

    // iterators
    function begin() -> iterator;
    function begin() const -> const_iterator ;
    function end() -> iterator;
    function end() const -> const_iterator;

    // capacity
    function empty() const noexcept -> bool;
    function length() const noexcept -> size_t;

    // modifiers
    virtual function clear() -> iterable<T, C>&;
    template <class F> function remove_if(F&& func, bool all = false) -> iterable<T, C>&;
    template <class F> function replace_if(F&& func, const T& new_item, bool all = false) -> iterable<T, C>&;
    function remove(const T& item, bool all = false) -> iterable<T, C>&;
    function replace(const T& old_item, const T& new_item, bool all = false) -> iterable<T, C>&;
    function reverse() -> iterable<T, C>&;
    function sort() -> iterable<T, C>&;
    function clean() -> iterable<T, C>& requires std::is_pointer_v<T>;

    // new iterables
    function reversed() const -> iterable<T, C>;
    function sorted() const -> iterable<T, C>;

    // algorithms
    function find(const T& object, const size_t offset = 0) const -> size_t;
    function contains(const T& item) const -> bool;
    function print() const -> void;

public: operators
    operator bool() const;
    function operator!() const -> bool;
    function operator==(const iterable<T, C>& o) const -> bool;
    function operator!=(const iterable<T, C>& o) const -> bool;

protected: internal_properties
    C m_iterable;
};


template <typename T, typename C>
function ext::iterable<T, C>::front() const noexcept(false) -> T&
{
    // throws error if accessing the front of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access front of an empty iterable"};

    // return the item at the front of the iterable
    return m_iterable.front();
}


template <typename T, typename C>
function ext::iterable<T, C>::back() const noexcept(false) -> T&
{
    // throws error if accessing the back of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access back of an empty iterable"};

    // return the item at the back of the iterable
    return m_iterable.back();
}


template <typename T, typename C>
function ext::iterable<T, C>::at(const size_t i) const noexcept(false) -> T&
{
    // throws error if accessing the middle of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access nth item of an empty iterable"};

    // return the item int the middle of the iterable
    return m_iterable.at(i);
}


template <typename T, typename C>
function ext::iterable<T, C>::at(const_iterator i) const noexcept(false) -> T&
{
    // throws error if accessing the middle of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access nth-iterator item of an empty iterable"};

    // return the item in the middle of the iterable
    return m_iterable.at(*i);
}


template <typename T, typename C>
function ext::iterable<T, C>::begin() -> ext::iterable<T, C>::iterator
{
    // return the begin iterator for the iterable
    return m_iterable.begin();
}


template <typename T, typename C>
function ext::iterable<T, C>::begin() const -> ext::iterable<T, C>::const_iterator
{
    // return the const begin iterator for the iterable
    return m_iterable.begin();
}


template <typename T, typename C>
function ext::iterable<T, C>::end() -> ext::iterable<T, C>::iterator
{
    // return the end iterator for the iterable
    return m_iterable.end();
}


template <typename T, typename C>
function ext::iterable<T, C>::end() const -> ext::iterable<T, C>::const_iterator
{
    // return the const end iterator for the iterable
    return m_iterable.end();
}


template <typename T, typename C>
inline bool ext::iterable<T, C>::empty() const noexcept
{
    // return if the iterable is empty or not
    return m_iterable.empty();
}


template <typename T, typename C>
inline size_t ext::iterable<T, C>::length() const noexcept
{
    // return the size of the iterable
    return m_iterable.size();
}


template <typename T, typename C>
inline ext::iterable<T, C>& ext::iterable<T, C>::clear()
{
    // clear the iterable, and return the reference to it
    m_iterable.clear();
    return *this;
}


template <typename T, typename C>
template <class function>
inline ext::iterable<T, C>& ext::iterable<T, C>::remove_if(function&& func, bool all)
{
    // continue to loop while the function matches items in the iterable
    while (std::find_if(begin(), end(), func) != end())
    {
        // remove the item, and continue looping if looking for all matches
        std::remove_if(begin(), end(), func);
        if (not all) break;
    }

    // return the point to the iterable
    return *this;
}


template <typename T, typename C>
template <class function>
inline ext::iterable<T, C>& ext::iterable<T, C>::replace_if(function&& func, const T& new_item, bool all)
{
    // continue to loop while the function matches items in the iterable
    while (std::find_if(begin(), end(), func) != end())
    {
        // replace the item, and continue looping if looking for all matches
        std::replace_if(begin(), end(), func, new_item);
        if (not all) break;
    }

    // return the reference to the iterable
    return *this;
}


template <typename T, typename C>
inline ext::iterable<T, C>& ext::iterable<T, C>::remove(const T& item, bool all)
{
    // continue to loop while the item is in the iterable
    while (contains(item))
    {
        // remove the item, and continue looping if looking for all matches
        std::remove(begin(), end(), item);
        if (not all) break;
    }

    // return the reference to the iterable
    return *this;
}


template <typename T, typename C>
inline ext::iterable<T, C>& ext::iterable<T, C>::replace(const T& old_item, const T& new_item, bool all)
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
inline ext::iterable<T, C>& ext::iterable<T, C>::reverse()
{
    // reverse the iterable, and return the reference to it
    std::reverse(m_iterable.begin(), m_iterable.end());
    return *this;
}


template <typename T, typename C>
inline ext::iterable<T, C>& ext::iterable<T, C>::sort()
{
    // sort the iterable, and return the reference to it
    std::sort(m_iterable.begin(), m_iterable.end());
    return *this;
}


template <typename T, typename C>
inline ext::iterable<C, T>& ext::iterable<T, C>::clean() requires std::is_pointer_v<T>
{
    // remove all the nullptr from the iterable, and return the reference to it
    remove(nullptr, true);
    return *this;
}


template <typename T, typename C>
inline ext::iterable<T, C> ext::iterable<T, C>::reversed() const
{
    // reverse a duplicate iterable, and return the reference to it
    return iterable<T, C>{*this}.reverse();
}


template <typename T, typename C>
inline ext::iterable<T, C> ext::iterable<T, C>::sorted() const
{
    // sort a duplicate iterable, and return the reference to it
    return iterable<T, C>{*this}.sort();
}


template <typename T, typename C>
size_t ext::iterable<T, C>::find(const T& object, const size_t offset) const
{
    // return the index of an item in the iterable by comparing iterator positions
    return std::distance(begin(), std::find(begin() + offset, end(), object));
}


template <typename T, typename C>
inline bool ext::iterable<T, C>::contains(const T& item) const
{
    // check if the iterable contains an item by comparing its iterator location to the end iterator
    return begin() + find(item) != end();
}


template <typename T, typename C>
void ext::iterable<T, C>::print() const
{
    // serialize the iterable by output the list as a string - TODO: MOVE TO OPERATOR <<
    std::cout << std::copy(begin(), end(), std::ostream_iterator<std::string>(std::cout, ", ")) << std::endl;
}


template <typename T, typename C>
bool ext::iterable<T, C>::operator!() const
{
    // the iterable evaluates to false if the list is empty (inverse operator ie -> true)
    return empty();
}


template <typename T, typename C>
bool ext::iterable<T, C>::operator==(const iterable<T, C>& o) const
{
    // guard to check that the lengths match
    if (length() != o.length())
        return false;

    // create a range of indexes the length of the iterable
    std::array<size_t, length()> range;
    std::iota(range.begin(), range.end(), 0);

    // equality check by comparing the items in the two iterables
    return std::all_of(range.begin(), range.end(), [this, o](const size_t i) {return at(i) == o.at(i);});
}


template <typename T, typename C>
bool ext::iterable<T, C>::operator!=(const iterable<T, C>& o) const
{
    // guard to check that the lengths don't match
    if (length() == o.length())
        return false;

    // create a range of indexes the length of the iterable
    std::array<size_t, length()> range;
    std::iota(range.begin(), range.end(), 0);

    // inequality check by comparing the items in the two iterables
    return std::any_of(range.begin(), range.end(), [this, o](const size_t i) {return at(i) != o.at(i);});
}


template <typename T, typename C>
ext::iterable<T, C>::operator bool() const
{
    // the iterable evaluates to true if the list has at least 1 item in
    return not empty();
}



#endif //SBROWSER_ITERABLE_HPP
