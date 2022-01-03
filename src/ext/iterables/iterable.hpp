#pragma once
#ifndef SBROWSER_ITERABLE_HPP
#define SBROWSER_ITERABLE_HPP

#include <algorithm>
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
    inline T& front() const {
        if (empty()) throw std::out_of_range{"Cannot access front of an empty iterable"};
        return const_cast<T&>(m_iterable.front());
    }

    inline T& back() const {
        if (empty()) throw std::out_of_range{"Cannot access back of an empty iterable"};
        return const_cast<T&>(m_iterable.back());
    }

    inline T& at(const size_t index) const {
        if (empty()) throw std::out_of_range{"Cannot access nth item of an empty iterable"};
        return const_cast<T&>(m_iterable.at(index));
    }

    inline T& at_iter(iterator index_iterator) const {
        if (empty()) throw std::out_of_range{"Cannot access nth iterator item of an empty iterable"};
        return const_cast<T&>(m_iterable.at(*index_iterator));
    }

    // iterators
    inline iterator begin() {
        return m_iterable.begin();
    }

    inline const_iterator begin() const {
        return m_iterable.begin();
    }

    inline iterator end() {
        return m_iterable.end();
    }

    inline const_iterator end() const {
        return m_iterable.end();
    }

    // capacity
    inline bool empty() const {
        return m_iterable.empty();
    }

    inline size_t length() const {
        return m_iterable.size();
    }

    // modifiers
    inline virtual iterable<T, C>& clear() {
        m_iterable.clear();
        return *this;
    }
    
    inline iterable<T, C>& remove(const T& item, bool all = false) {
        while (contains(item)) {
            static_cast<void>(std::remove(begin(), end(), item)); // TODO : huge template warnings without 'auto r = ' ...
            if (not all) break;
        }
        return *this;
    }

    template <class function> inline iterable<T, C>& remove_if(function&& func, bool all = false) {
        while (std::find_if(begin(), end(), func) != end()) {
            static_cast<void>(std::remove_if(begin(), end(), func));
            if (not all) break;
        }
        return *this;
    }

    inline iterable<T, C>& replace(const T& old_item, const T& new_item, bool all = false) {
        while (contains(old_item)) {
            std::replace(begin(), end(), old_item, new_item);
            if (not all) break;
        }
        return *this;
    }

    template <class function> inline iterable<T, C>& replace_if(function&& func, const T& new_item, bool all = false) {
        while (std::find_if(begin(), end(), func) != end()) {
            std::replace_if(begin(), end(), func, new_item);
            if (not all) break;
        }
        return *this;
    }
    
    inline iterable<T, C>& reverse() {
        std::reverse(m_iterable.begin(), m_iterable.end());
        return *this;
    }

    inline iterable<T, C>& sort() {
        std::sort(m_iterable.begin(), m_iterable.end());
        return *this;
    }

    inline iterable<C, T>& clean() requires std::is_pointer_v<T> {
        return remove(nullptr, true);
    }

    // new iterables
    inline iterable<T, C> reversed() const {
        return iterable<T, C>{*this}.reverse();
    }

    inline iterable<T, C>& sorted() const {
        return iterable<T, C>{*this}.sort();
    }

    // algorithms
    inline size_t find(const T& object, const size_t offset = 0) const {
        return std::distance(begin(), std::find(begin() + offset, end(), object));
    }

    inline bool contains(const T& item) const {
        return begin() + find(item) != end();
    }

public: operators
    inline bool operator not() const {
        return empty();
    }

    inline bool operator==(const iterable<T, C>& other) const {
        return m_iterable == other.m_iterable;
    }

    inline bool operator!=(const iterable<T, C>& other) const {
        return m_iterable != other.m_iterable;
    }

    inline operator bool() const {
        return not empty();
    }

protected: internal_properties
    C m_iterable;
};


#endif //SBROWSER_ITERABLE_HPP
