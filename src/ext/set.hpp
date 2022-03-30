#ifndef SBROWSER_SET_HPP
#define SBROWSER_SET_HPP

#include <set>
#include <ext/iterable.hpp>

namespace ext {template <typename T> class set;}


template <typename T>
class ext::set final : public ext::iterable<T, std::set<T>>
{
public constructors:
    set() = default;
    explicit set(const std::set<T>& o);

public js_methods:
    auto add(const T& item) -> ext::set<T>&;
};


template <typename T>
ext::set<T>::set(const std::set<T>& o)
{
    // set the iterable to a set
    this->m_iterable = o;
}


template <typename T>
auto ext::set<T>::add(const T& item) -> ext::set<T>&
{
    // insert the item in the middle of the iterable, and return the reference to the iterable
    this->m_iterable.emplace(item);
    return *this;
}


#endif //SBROWSER_SET_HPP
