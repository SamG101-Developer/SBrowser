#pragma once
#ifndef SBROWSER_ITERABLE_HPP
#define SBROWSER_ITERABLE_HPP

#include <algorithm>

namespace ext {template <typename T, typename C> class iterable;}


template <typename T, typename C>
class ext::iterable {
public:
    iterable() = default;
    iterable(const iterable<T, C>&) = default;
    iterable(iterable<T, C>&&) noexcept = default;
    iterable<T, C>& operator=(const iterable<T, C>&) = default;
    iterable<T, C>& operator=(iterable<T, C>&&) noexcept = default;

    virtual ~iterable() = default;
    
    using iterator       = typename C::iterator;
    using const_iterator = typename C::const_iterator;

    inline std::size_t length() const {return m_iterable.size();}
    inline bool empty() const {return m_iterable.empty();}

    inline T front() const {return empty() ? std::is_pointer_v<T> ? nullptr : T() : m_iterable.front();}
    inline T back() const {return empty() ? std::is_pointer_v<T> ? nullptr : T() : m_iterable.back();}
    inline T at(const std::size_t index) const {return m_iterable.at(index);}
    inline T at_iter(const iterator& index_iterator) const {return m_iterable.at(std::distance(begin(), index_iterator));}
    inline std::size_t find(const T& object, const std::size_t offset = 0) const {return std::distance(begin(), std::find(begin() + offset, end(), object));}
    inline bool contains(const T& item) const {return begin() + find(item) != end();}

    inline iterable<T, C>& clear(bool delete_pointers = false) {
        m_iterable.clear();
        m_iterable.shrink_to_fit();
        return *this;
    }
    
    inline iterable<T, C>& remove(const T& item, bool all = false) {
        while (contains(item)) {
            std::remove(begin(), end(), item);
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

    template <class function> inline iterable<T, C>& remove_if(function&& func, bool all = false) {
        while (std::find_if(begin(), end(), func) != end()) {
            std::remove_if(begin(), end(), func);
            if (not all) break;
        }
        return *this;
    }

    template <class function> inline iterable<T, C>& replace_if(function&& func, const T& new_item, bool all = false) {
        while (std::find(begin(), end(), func) != end()) {
            std::replace_if(begin(), end(), func, new_item);
            if (not all) break;
        }
        return *this;
    }
    
    inline iterable<T, C>& reverse() {
        std::reverse(m_iterable.begin(), m_iterable.end());
        return *this;
    }

    inline iterable<T, C> reversed() {
        return iterable<T, C>{*this}.reverse();
    }

    inline iterable<T, C>& sort() {
        std::sort(m_iterable.begin(), m_iterable.end());
        return *this;
    }

    inline iterable<T, C>& sorted() {
        return iterable<T, C>{*this}.sort();
    }
    
    inline iterator begin() {return m_iterable.begin();}
    inline iterator end()   {return m_iterable.end();}
    inline const_iterator begin() const {return m_iterable.begin();}
    inline const_iterator end()   const {return m_iterable.end();}

    inline iterable<C, T>& clean() requires std::is_pointer_v<T> {
        return remove(nullptr, true);
    }

    inline bool operator not() {return empty();}

    inline bool operator==(const iterable<T, C>& other) {return m_iterable == other.m_iterable;}
    inline bool operator!=(const iterable<T, C>& other) {return m_iterable != other.m_iterable;}
    inline operator bool() const {return not empty();}

protected:
    C m_iterable;
};


#endif //SBROWSER_ITERABLE_HPP
