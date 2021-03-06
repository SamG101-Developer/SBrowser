#pragma once
#ifndef SBROWSER_ITERABLE_HPP
#define SBROWSER_ITERABLE_HPP

#include <ext/container_live_linker.hpp>

#include <algorithm>
#include <format>
#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <numeric>
#include <stdexcept>
#include <type_traits>

#include <ext/cpp_keywords.hpp>
#include <ext/decorators.hpp>
#include <ext/optional.hpp>

namespace ext {template <typename T, typename C> class iterable;}


template <typename T, typename C>
class ext::iterable
        : public container_live_filter_linker<C, iterable<T, C>>
{
public aliases:
    using iterator               = typename C::iterator;               // points to item
    using const_iterator         = typename C::const_iterator;         // points to const item (!= const iterator)
    using reverse_iterator       = typename C::reverse_iterator;       // points to item in reverse
    using const_reverse_iterator = typename C::const_reverse_iterator; // points to const item in reverse

public constructors:
    iterable() = default;
    iterable(const iterable&) = default;
    iterable(iterable&&) noexcept = default;
    auto operator=(const iterable&) -> iterable& = default;
    auto operator=(iterable&&) noexcept -> iterable& = default;
    virtual ~iterable() = default;

    // iterator access
    auto front() noexcept(false) -> optional<T>;
    auto back() noexcept(false) -> optional<T>;
    auto at(iterator where) const noexcept(false) -> optional<T>;

    // get begin and end iterators (forward, const, reverse, const-reverse)
    auto begin() -> iterator;
    auto begin() const -> const_iterator;
    auto rbegin() -> reverse_iterator;
    auto rbegin() const -> const_reverse_iterator;
    auto cbegin() const -> const_iterator;
    auto crbegin() const -> const_reverse_iterator;

    auto end() -> iterator;
    auto end() const -> const_iterator;
    auto rend() -> reverse_iterator;
    auto rend() const -> const_reverse_iterator;
    auto cend() const -> const_iterator;
    auto crend() const -> const_reverse_iterator;

    // number of elements in the list
    [[nodiscard]] auto empty() const noexcept -> bool;
    [[nodiscard]] auto length() const noexcept -> size_t;

    // removing items from the list
    virtual auto clear() -> iterable&;
    virtual auto clean() -> iterable&;

    template <bool all, typename ...args> auto remove(args&&... items) -> iterable&;
    template <bool all, class F> auto remove_if(F&& function) -> iterable&;

    template <bool all> auto replace(T&& old_item, T&& new_item) -> iterable&;
    template <bool all, class F> auto replace_if(F&& function, T&& new_item) -> iterable&;

    template <typename ...args> auto ltrim(args&&... item) -> iterable&;
    template <typename ...args> auto rtrim(args&&... item) -> iterable&;
    template <typename ...args> auto trim(args&&... item) -> iterable&;

    // modifying this iterable and creating a modified iterable
    auto reverse() -> iterable&;
    auto reversed() const -> iterable;
    auto sort() -> iterable&;
    auto sorted() const -> iterable;

    // get the iterator where the item is in the list
    auto find(const T& item, const_iterator offset = nullptr) const -> iterator;
    template <typename F> auto find_if(F&& function, const iterator offset = nullptr) const -> iterator;
    template <typename F> auto find_if_not(F&& function, const iterator offset = nullptr) const -> iterator;

    template <typename ...args> auto find_first_that_is(args&&... items) const -> iterator;
    template <typename F> auto find_first_that_is_if(F&& function) const -> iterator;

    template <typename ...args> auto find_first_that_isnt(args&&... items) const -> iterator;
    template <typename F> auto find_first_that_isnt_if(F&& function) const -> iterator;

    template <typename ...args> auto find_last_that_is(args&&... items) const -> iterator;
    template <typename F> auto find_last_that_is_if(F&& function) const -> iterator;

    template <typename ...args> auto find_last_that_isnt(args&&... items) const -> iterator;
    template <typename F> auto find_last_that_isnt_if(F&& function) const -> iterator;

    // other methods
    template <typename ...args> auto count(const args&... item) const -> size_t;
    template <typename ...args> auto contains(const args&... item) const -> bool;

public operators:
    virtual auto operator!() const -> bool;
    auto operator==(const iterable& that) const -> bool;

protected cpp_properties:
    C m_iterable;
};


template <typename T, typename C>
auto ext::iterable<T, C>::front() noexcept(false)
        -> optional<T>
{
    // return the item at the font of the iterable, if it exists
    auto found_front_item = ext::optional<T>{};
    if (not empty()) found_front_item.template emplace(m_iterable.front());
    return found_front_item;
}


template <typename T, typename C>
auto ext::iterable<T, C>::back() noexcept(false)
        -> optional<T>
{
    // return the item at the back of the iterable, if it exists
    auto found_last_item = ext::optional<T>{};
    if (not empty()) found_last_item.template emplace(m_iterable.back());
    return found_last_item;
}


template <typename T, typename C>
auto ext::iterable<T, C>::at(
        const iterator where) const noexcept(false)
        -> optional<T>
{
    // return the item in the middle of the iterable, if it exists
    auto found_at_item = ext::optional<T>{};
    if (not empty() and where > begin() and where < end()) found_at_item.template emplace(*where);
    return found_at_item;
}


template <typename T, typename C>
auto ext::iterable<T, C>::begin()
        -> iterator
{
    // return the begin iterator for the iterable
    return m_iterable.begin();
}


template <typename T, typename C>
auto ext::iterable<T, C>::cbegin() const
        -> const_iterator
{
    // return the const begin iterator for the iterable
    return m_iterable.begin();
}


template <typename T, typename C>
auto ext::iterable<T, C>::rbegin()
        -> reverse_iterator
{
    // return the reverse begin iterator
    return m_iterable.rbegin();
}


template <typename T, typename C>
auto ext::iterable<T, C>::crbegin() const
        -> const_reverse_iterator
{
    // return the const reverse begin iterator
    return m_iterable.crbegin();
}


template <typename T, typename C>
auto ext::iterable<T, C>::end()
        -> iterator
{
    // return the end iterator for the iterable
    return m_iterable.end();
}


template <typename T, typename C>
auto ext::iterable<T, C>::cend() const
        -> const_iterator
{
    // return the const end iterator for the iterable
    return m_iterable.end();
}


template <typename T, typename C>
auto ext::iterable<T, C>::rend()
        -> reverse_iterator
{
    // return the reverse end iterator
    return m_iterable.rend();
}


template <typename T, typename C>
auto ext::iterable<T, C>::crend() const
        -> const_reverse_iterator
{
    // return the const reverse end iterator
    return m_iterable.crend();
}


template <typename T, typename C>
auto ext::iterable<T, C>::empty() const noexcept
        -> bool
{
    // return if the iterable is empty or not
    return m_iterable.empty();
}


template <typename T, typename C>
auto ext::iterable<T, C>::length() const noexcept
        -> size_t
{
    // return the size of the iterable
    return m_iterable.size();
}


template <typename T, typename C>
auto ext::iterable<T, C>::clear()
        -> iterable&
{
    // clear the iterable, and return the reference to it
    m_iterable.clear();
    return *this;
}


template <typename T, typename C>
auto ext::iterable<T, C>::clean()
        -> iterable&
{
    // remove all the nullptr from the iterable, and return the reference to it
    LIVE_FILTER_MUTABLE(clean);
    if constexpr(std::is_pointer_v<T>) remove<true>(nullptr);
    return *this;
}


template <typename T, typename C>
template <bool all, typename ...args>
auto ext::iterable<T, C>::remove(
        args&& ...items)
        -> iterable&
{
    // TODO LIVE_FILTER_MUTABLE(remove<all>, std::forward<args>(items)...);
    return remove_if<all>([&items...](const T& item) {return ((item == std::forward<args>(items)) || ...);});
}


template <typename T, typename C>
template <bool all, typename F>
auto ext::iterable<T, C>::remove_if(
        F&& function)
        -> iterable&
{
    // TODO do {std::remove_if(begin(), end(), function);} while (all and function());
    // TODO LIVE_FILTER_MUTABLE
    return *this;
}


template <typename T, typename C>
template <bool all>
auto ext::iterable<T, C>::replace(
        T&& old_item,
        T&& new_item)
        -> iterable&
{
    // TODO LIVE_FILTER_MUTABLE
    return replace_if<all>([this, old_item = std::move(old_item)](const T& item) {return item == old_item;}, std::move(new_item));
}


template <typename T, typename C>
template <bool all, typename F>
auto ext::iterable<T, C>::replace_if(
        F&& function,
        T&& new_item)
        -> iterable&
{
    // TODO LIVE_FILTER_MUTABLE
    do {std::replace_if(begin(), end(), function, std::move(new_item));} while (all and function());
    return *this;
}


template <typename T, typename C>
template <typename ...args>
auto ext::iterable<T, C>::ltrim(
        args&&... item)
        -> iterable&
{
    // remove all the items from the left-hand side of the iterable, and return the reference to the iterable
    LIVE_FILTER_MUTABLE(ltrim, std::forward<args>(item)...);
    (m_iterable.erase(
            begin(),
            begin() + std::distance(begin(), std::find_if_not(begin(), end(), [item = std::forward<args>(item)](T element) {return element == item;}))), ...);
    return *this;
}


template <typename T, typename C>
template <typename ...args>
auto ext::iterable<T, C>::rtrim(
        args&&... item)
        -> iterable&
{
    // remove all the items from the right-hand side of the iterable, and return the reference to the iterable
    LIVE_FILTER_MUTABLE(rtrim, std::forward<args>(item)...);
    (m_iterable.erase(
            begin() + std::distance(rbegin(), std::find_if_not(rbegin(), rend(), [item = std::forward<args>(item)](T element) {return element == item;})),
            end()), ...);
    return *this;
}


template <typename T, typename C>
template <typename ...args>
auto ext::iterable<T, C>::trim(
        args&&... item)
        -> iterable&
{
    // remove all the items from both sides of the iterable, and return the reference to the iterable
    LIVE_FILTER_MUTABLE(trim, std::forward<args>(item)...);
    ltrim(std::forward<args>(item)...);
    rtrim(std::forward<args>(item)...);
    return *this;
}


template <typename T, typename C>
auto ext::iterable<T, C>::reverse()
        -> iterable&
{
    // reverse the iterable, and return the reference to it
    LIVE_FILTER_MUTABLE(reverse);
    std::ranges::reverse(m_iterable.begin(), m_iterable.end());
    return *this;
}


template <typename T, typename C>
auto ext::iterable<T, C>::reversed() const
        -> iterable
{
    // reverse a duplicate iterable, and return the reference to it
    return iterable{*this}.reverse();
}


template <typename T, typename C>
auto ext::iterable<T, C>::sort()
        -> iterable&
{
    // sort the iterable, and return the reference to it
    LIVE_FILTER_MUTABLE(sort);
    std::sort(m_iterable.begin(), m_iterable.end());
    return *this;
}


template <typename T, typename C>
auto ext::iterable<T, C>::sorted() const
        -> iterable
{
    // sort a duplicate iterable, and return the reference to it
    return iterable{*this}.sort();
}


template <typename T, typename C>
auto ext::iterable<T, C>::find(
        const T& item,
        const_iterator offset) const
        -> iterator
{
    // return the index of an item in the iterable by comparing iterator positions
    return std::find(offset, cend(), item);
}


template <typename T, typename C>
template <typename F>
auto ext::iterable<T, C>::find_if(
        F&& function,
        const iterator offset) const
        -> iterator
{
    return std::find_if(begin() + offset, end(), std::forward<T>(function));
}


template <typename T, typename C>
template <typename F>
auto ext::iterable<T, C>::find_if_not(
        F&& function,
        const iterator offset) const
        -> iterator
{
    return std::find_if_not(begin() + offset, end(), std::forward<F>(function));
}


template <typename T, typename C>
template <typename ...args>
auto ext::iterable<T, C>::find_first_that_is(
        args&&... items) const
        -> iterator
{
    // find the first instance of a specific item
    return find_first_that_is_if([items...](const T& item) {return ((item == std::forward<args>(items)) || ...);});
}


template <typename T, typename C>
template <typename F>
auto ext::iterable<T, C>::find_first_that_is_if(
        F&& function) const
        -> iterator
{
    // find the first instance of a function compliant item
    return std::find_first_of(begin(), end(), std::forward<F>(function));
}


template <typename T, typename C>
template <typename ...args>
auto ext::iterable<T, C>::find_first_that_isnt(
        args&&... items) const
        -> iterator
{
    // find the first non-instance of a specific item
    return find_first_that_isnt([items...](const T& item) {return ((item != std::forward<args>(items)) && ...);});
}


template <typename T, typename C>
template <typename F>
auto ext::iterable<T, C>::find_first_that_isnt_if(
        F&& function) const
        -> iterator
{
    // find the first non-instance of a function compliant item
    return std::find_first_of(begin(), end(), [function=std::forward<F>(function)] {not function();});
}


template <typename T, typename C>
template <typename ...args>
auto ext::iterable<T, C>::find_last_that_is(
        args&&... items) const
        -> iterator
{
    // find the last instance of a specific item
    return find_last_that_is_if([items...](const T& item) {return ((item == std::forward<args>(items)) || ...);});
}


template <typename T, typename C>
template <typename F>
auto ext::iterable<T, C>::find_last_that_is_if(
        F&& function) const
        -> iterator
{
    // find the last instance of a function compliant item
    return std::find_first_of(rbegin(), rend(), std::forward<F>(function));
}


template <typename T, typename C>
template <typename ...args>
auto ext::iterable<T, C>::find_last_that_isnt(
        args&&... items) const
        -> iterator
{
    // find the last non-instance of a specific item
    return find_last_that_isnt_if([items...](const T& item) {return ((item != std::forward<args>(items)) || ...);});
}


template <typename T, typename C>
template <typename F>
auto ext::iterable<T, C>::find_last_that_isnt_if(
        F&& function) const
        -> iterator
{
    // find the last non-instance of a function compliant item
    return std::find_first_of(rbegin(), rend(), [function=std::forward<F>(function)] {not function();});
}


template <typename T, typename C>
template <typename ...args>
auto ext::iterable<T, C>::count(
        const args&... item) const
        -> size_t
{
    // return the number of occurrences of an item
    return (std::ranges::count(m_iterable.begin(), m_iterable.end(), item) + ...);
}


template <typename T, typename C>
template <typename ...args>
auto ext::iterable<T, C>::contains(
        const args&... item) const
        -> bool
{
    // check if the iterable contains an item by comparing its iterator location to the end iterator
    return ((std::find(begin(), end(), item) != cend()) || ...);
}


template <typename T, typename C>
auto ext::iterable<T, C>::operator!() const
        -> bool
{
    // the iterable evaluates to false if the list is empty (inverse operator ie -> true)
    return empty();
}


template <typename T, typename C>
auto ext::iterable<T, C>::operator==(
        const iterable& that) const
        -> bool
{
    // guard to check that the lengths match
    if (length() != that.length())
        return false;

    // determine the end iterator conditions
    auto a_end = end();
    auto b_end = that.end();

    // iterate and compare elements
    for (
            auto a_iterator = begin(), b_iterator = that.begin();
            a_iterator != a_end and b_iterator != b_end;
            ++a_iterator, ++b_iterator)
    {
        if (*a_iterator != *b_iterator)
            return false;
    }
}


class A{public: A(){}};


template class ext::iterable<int, std::vector<int>>;
template class ext::iterable<A  , std::vector<A>>;
template class ext::iterable<A* , std::vector<A*>>;



#endif //SBROWSER_ITERABLE_HPP
