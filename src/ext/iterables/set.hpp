#ifndef SBROWSER_SET_HPP
#define SBROWSER_SET_HPP

#include <set>

#include <ext/iterables/iterable.hpp>

namespace ext {template <typename T> class set;}


template <typename T>
class ext::set : public ext::iterable<T, std::set<T>> {
public: constructors
    set() = default;

public: methods
    // modifiers
    ext::set<T>& insert(const T& item);
};


template <typename T>
ext::set<T>& ext::set<T>::insert(const T& item)
{
    // insert the item in the middle of the iterable, and return the reference to the iterable
    this->m_iterable.emplace(item);
    return *this;
}


#endif //SBROWSER_SET_HPP
