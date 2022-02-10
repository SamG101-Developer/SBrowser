#pragma once
#ifndef SBROWSER_ITERABLE_HPP
#define SBROWSER_ITERABLE_HPP

#include <algorithm>
#include <iostream>
#include <stdexcept>

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
    T& front() const noexcept(false);
    T& back() const noexcept(false);
    T& at(const size_t index) const noexcept(false);
    T& at(const_iterator index) const noexcept(false);

    // iterators
    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    // capacity
    bool empty() const noexcept;
    size_t length() const noexcept;

    // modifiers
    virtual iterable<T, C>& clear();
    template <class function> iterable<T, C>& remove_if(function&& func, bool all = false);
    template <class function> iterable<T, C>& replace_if(function&& func, const T& new_item, bool all = false);
    iterable<T, C>& remove(const T& item, bool all = false);
    iterable<T, C>& replace(const T& old_item, const T& new_item, bool all = false);
    iterable<T, C>& reverse();
    iterable<T, C>& sort();
    iterable<C, T>& clean() requires std::is_pointer_v<T>;

    // new iterables
    iterable<T, C> reversed() const;
    iterable<T, C> sorted() const;

    // algorithms
    size_t find(const T& object, const size_t offset = 0) const;
    bool contains(const T& item) const;
    void print() const;

public: operators
    operator bool() const;
    bool operator!() const;
    bool operator==(const iterable<T, C>& other) const;
    bool operator!=(const iterable<T, C>& other) const;

protected: internal_properties
    C m_iterable;
};


template <typename T, typename C>
inline T& ext::iterable<T, C>::front() const noexcept(false)
{
    // throws error if accessing the front of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access front of an empty iterable"};

    // return the front of the iterable
    return m_iterable.front();
}


template <typename T, typename C>
inline T& ext::iterable<T, C>::back() const noexcept(false)
{
    // throws error if accessing the back of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access back of an empty iterable"};

    // return the back of the iterable
    return m_iterable.back();
}


template <typename T, typename C>
inline T& ext::iterable<T, C>::at(const size_t index) const noexcept(false)
{
    // throws error if accessing the middle of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access nth item of an empty iterable"};

    // return the middle of the iterable
    return m_iterable.at(index);
}


template <typename T, typename C>
inline T& ext::iterable<T, C>::at(const_iterator index) const noexcept(false)
{
    // throws error if accessing the middle of an empty iterable
    if (empty())
        throw std::out_of_range{"Cannot access nth iterator item of an empty iterable"};

    // return the middle of the iterable
    return m_iterable.at(*index);
}


template <typename T, typename C>
inline typename ext::iterable<T, C>::iterator ext::iterable<T, C>::begin()
{
    // return the begin iterator for the iterable
    return m_iterable.begin();
}


template <typename T, typename C>
inline typename ext::iterable<T, C>::const_iterator ext::iterable<T, C>::begin() const
{
    // return the const begin iterator for the iterable
    return m_iterable.begin();
}


template <typename T, typename C>
inline typename ext::iterable<T, C>::iterator ext::iterable<T, C>::end()
{
    // return the end iterator for the iterable
    return m_iterable.end();
}


template <typename T, typename C>
inline typename ext::iterable<T, C>::const_iterator ext::iterable<T, C>::end() const
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
    // clear the iterable and return the pointer to it
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


/***
 * Replace items in the iterable if they match a function ie the function being run on the item returns True (function
 * must have a boolean (-castable) output. Can either remove all elements matching the function or just the first match.
 * Every matching item is replaced by new_item.
 * @tparam T Type of element
 * @tparam C Type of container
 * @tparam function Type of function
 * @param func Function to match elements against
 * @param new_item Item to replace with
 * @param all If all matching items should be replaces
 * @return This iterable
 */
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

    // return the pointer to the iterable
    return *this;
}


template <typename T, typename C>
inline ext::iterable<T, C>& ext::iterable<T, C>::remove(const T& item, bool all)
{
    while (contains(item)) {
        static_cast<void>(std::remove(begin(), end(), item)); // TODO : huge template warnings without 'auto r = ' ...
        if (not all) break;
    }
    return *this;
}


template <typename T, typename C>
inline ext::iterable<T, C>& ext::iterable<T, C>::replace(const T& old_item, const T& new_item, bool all)
{
    while (contains(old_item)) {
        std::replace(begin(), end(), old_item, new_item);
        if (not all) break;
    }
    return *this;
}


template <typename T, typename C>
inline ext::iterable<T, C>& ext::iterable<T, C>::reverse()
{
    std::reverse(m_iterable.begin(), m_iterable.end());
    return *this;
}


template <typename T, typename C>
inline ext::iterable<T, C>& ext::iterable<T, C>::sort()
{
    std::sort(m_iterable.begin(), m_iterable.end());
    return *this;
}


template <typename T, typename C>
inline ext::iterable<C, T>& ext::iterable<T, C>::clean() requires std::is_pointer_v<T>
{
    return remove(nullptr, true);
}


template <typename T, typename C>
inline ext::iterable<T, C> ext::iterable<T, C>::reversed() const
{
    return iterable<T, C>{*this}.reverse();
}


template <typename T, typename C>
inline ext::iterable<T, C> ext::iterable<T, C>::sorted() const
{
    return iterable<T, C>{*this}.sort();
}


template <typename T, typename C>
size_t ext::iterable<T, C>::find(const T& object, const size_t offset) const
{
    return std::distance(begin(), std::find(begin() + offset, end(), object));
}


template <typename T, typename C>
inline bool ext::iterable<T, C>::contains(const T& item) const
{
    return begin() + find(item) != end();
}


template <typename T, typename C>
void ext::iterable<T, C>::print() const
{
    std::cout << std::copy(begin(), end(), std::ostream_iterator<std::string>(std::cout, ", ")) << std::endl;
}


template <typename T, typename C>
bool ext::iterable<T, C>::operator!() const
{
    return empty();
}


template <typename T, typename C>
bool ext::iterable<T, C>::operator==(const iterable<T, C>& other) const
{
    return m_iterable == other.m_iterable;
}


template <typename T, typename C>
bool ext::iterable<T, C>::operator!=(const iterable<T, C>& other) const
{
    return m_iterable != other.m_iterable;
}


template <typename T, typename C>
ext::iterable<T, C>::operator bool() const
{
    return not empty();
}



#endif //SBROWSER_ITERABLE_HPP
