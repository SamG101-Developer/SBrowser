#pragma once
#ifndef SBROWSER_VECTOR_HPP
#define SBROWSER_VECTOR_HPP

#include <algorithm>
#include <initializer_list>
#include <queue>
#include <stack>
#include <string>

#include <ext/iterable.hpp>
#include <ext/set.hpp>
#include <ext/type_traits.hpp>

#include <veque/include/veque.hpp>

namespace ext {template <typename T> class vector;}
namespace ext {class string;}
namespace ext
{
    using int8_array_t = ext::vector<char>;
    using int16_array_t = ext::vector<short>;
    using int32_array_t = ext::vector<int>;
    using int_64_array_t = ext::vector<long long>;

    using uint8_array_t = ext::vector<unsigned char>;
    using uint16_array_t = ext::vector<unsigned short>;
    using uint32_array_t = ext::vector<unsigned int>;
    using uint_64_array_t = ext::vector<unsigned long long>;

    using float_32_array_t = ext::vector<float>;
    using float_64_array_t = ext::vector<double>;
}


//    namespace {template <typename T, class = void> struct is_unique_ptr : std::false_type {};}
//    namespace {template <typename T> struct is_unique_ptr<std::unique_ptr<T>> : std::true_type {};}
//    namespace {template <typename T> constexpr bool is_shared_ptr_v = is_unique_ptr<T>::value;}


template <typename T>
class ext::vector : public iterable<T, veque::veque<T>>
{
public aliases:
    using iterator               = typename veque::veque<T>::iterator;
    using const_iterator         = typename veque::veque<T>::const_iterator;
    using reverse_iterator       = typename veque::veque<T>::reverse_iterator;
    using const_reverse_iterator = typename veque::veque<T>::const_reverse_iterator;

    using value_type = T;

public constructors:
    vector() = default;
    vector(const vector&) = default;
    vector(vector&&) noexcept = default;
    template <typename iterable_t> vector(const iterable_t& iterable_object) requires ext::is_iterable_v<iterable_t>;
    template <typename iterator_t> vector(iterator_t begin, iterator_t end) requires ext::is_iterator_v<iterator_t>;
    template <typename ...args> vector(args&&... items);

    auto operator=(const vector&) -> vector& = default;
    auto operator=(vector&&) noexcept -> vector& = default;
    auto operator=(const std::queue<T>& that) -> vector&;
    auto operator=(const std::stack<T>& that) -> vector&;

    ~vector() override;

public js_methods:
    auto slice(const iterator& front_index, const iterator& back_index) const -> vector;
    auto item_before(const T& item) const -> ext::optional<T>;
    auto item_after(const T& item) const -> ext::optional<T>;
    template <typename F> auto first_match(F&& function) const -> ext::optional<T>;
    template <typename F> auto last_match(F&& function) const -> ext::optional<T>;

    auto reserve(size_t count) -> vector&;
    auto shrink_to_fit() -> vector&;
    template <typename ...args> auto append(args&&... item) -> vector&;
    template <typename ...args> auto prepend(args&&... item) -> vector&;
    template <typename ...args> auto insert(iterator where, args&&... item) -> vector&;
    auto extend(const vector& other, iterator where) -> vector&;
    auto pop(const iterator where) -> T&;

    auto clear() -> vector& override;
    auto remove_all_before(iterator where) -> vector&;
    auto remove_all_after(iterator where) -> vector&;

    auto max_element() const -> T&;
    auto min_element() const -> T&;

    template <typename F> auto all_of(F&& function) const -> bool;
    template <typename F> auto any_of(F&& function) const -> bool;
    template <typename F> auto for_each(F&& function) -> vector&;
    template <typename F> auto for_each(F&& function) const -> const vector&;
    template <typename F> auto filter(F&& function) const -> vector;
    template <typename U=T, typename F> auto transform(F&& function) const -> vector<U>;
    template <typename U=T> auto cast_all() const -> vector<U>;
    auto difference(vector& other) const -> vector;

    auto intersection(vector& other) const -> vector&;
    auto join(char&& delimiter = ' ') const -> const char*;
    template <typename U=std::remove_extent_t<T>> auto flatten() -> vector<U>;
    auto call_all() const -> void requires std::is_invocable_v<T>;

    template <typename F> auto attach_live_filtered(vector* live_iterable, F&& filter_lambda) -> void;

public operators:
    auto operator*(size_t n) const -> vector;
};


template <typename T>
template <typename iterable_t>
ext::vector<T>::vector(
        const iterable_t& iterable_object)
        requires ext::is_iterable_v<iterable_t>
{
    // set the iterable to a veque from the iterator pair of the iterable_object
    this->m_iterable = veque::veque<T>{iterable_object.begin(), iterable_object.end()};
}


template <typename T>
template <typename iterator_t>
ext::vector<T>::vector(
        iterator_t begin,
        iterator_t end)
        requires ext::is_iterator_v<iterator_t>
{
    // set the iterable to a veque defined by an iterator pair
    this->m_iterable = veque::veque<T>{begin, end};
}


template <typename T>
template <typename ...args> ext::vector<T>::vector(
        args&&... items)
{
    // set the iterable to a veque defined by a variable number of arguments
    this->m_iterable = veque::veque<T>{};
    this->m_iterable.reserve(sizeof...(items));
    this->m_iterable.emplace_back(std::forward<args>(items)...);
    this->m_iterable.shrink_to_fit();
}


template <typename T>
auto ext::vector<T>::operator=(
        const std::queue<T>& that)
        -> vector&
{
    // fill the veque by emptying the queue#
    clear();
    auto copy = std::queue<T>{that};

    while (not copy.empty())
    {
        // append the front element and pop it from the queue
        append(copy.front());
        copy.pop();
    }
}


template <typename T>
auto ext::vector<T>::operator=(
        const std::stack<T>& that)
        -> vector&
{
    // fill the veque by emptying the stack
    clear();
    auto copy = std::stack<T>{that};

    while (not copy.empty())
    {
        // append the top element and pop it from the stack
        append(copy.top());
        copy.pop();
    }
}


template <typename T>
ext::vector<T>::~vector()
{
    // call the clear method to clear up the list before it is deleted
    clear();
}


template <typename T>
auto ext::vector<T>::slice(
        const iterator& front_index,
        const iterator& back_index) const
        -> vector
{
    // slice the list by adding the front_index and back_index to the begin iterator
    return vector<T>{front_index, back_index};
}


template <typename T>
auto ext::vector<T>::item_before(
        const T& item) const
        -> ext::optional<T>
{
    // get the item in the veque found adjacent to the item passed in, before
    auto where_item = this->find(item);
    auto found_item_before = ext::optional<T>{};
    if (where_item == this->begin())
        found_item_before.emplace(this->at(where_item - 1).value());
    return found_item_before;
}


template <typename T>
auto ext::vector<T>::item_after(
        const T& item) const
        -> ext::optional<T>
{
    // get the item in the veque found adjacent to the item passed in, after
    return this->at((this->find(item) + 1) % this->length());
}


template <typename T>
template <typename F>
auto ext::vector<T>::first_match(
        F&& function) const
        -> optional<T>
{
    // get the first match by filtering the veque and returning the first item
    // TODO : break on first item
    return filter(function).front();
}


template <typename T>
template <typename F>
auto ext::vector<T>::last_match(
        F&& function) const
        -> optional<T>
{
    // get the last match by filtering the veque and returning the last match
    // TODO : break on last item (reverse) OR better: filter or reverse_iterators and select first() (break after first)
    return filter(function).back();
}


template <typename T>
auto ext::vector<T>::reserve(
        size_t count)
        -> vector<T>&
{
    // reserve count amount of object space in the veque for quicker insertion, and return a reference to it
    this->m_iterable.reserve(count);
    return *this;
}


template <typename T>
auto ext::vector<T>::shrink_to_fit(
        ) -> vector<T>&
{
    // shrink the empty reserved space around the number of objects in the veque, and return a reference to it
    this->m_iterable.shrink_to_fit();
    return *this;
}


template <typename T>
template <typename ...args>
auto ext::vector<T>::append(
        args&&... item)
        -> vector&
{
    // append the item to the back of the veque, and return a reference to it
    LIVE_FILTER_MUTATE_ADD_EVENT(append, item);
    this->m_iterable.emplace_back(std::forward<args>(item)...);
    return *this;
}


template <typename T>
template <typename ...args>
auto ext::vector<T>::prepend(
        args&&... item)
        -> vector&
{
    // prepend the item to the front of the veque, and return a reference to it
    LIVE_FILTER_MUTABLE(prepend, std::forward<T>(item)...);
    this->m_iterable.emplace_front(std::forward<args>(item)...);
    return *this;
}


template <typename T>
template <typename ...args>
auto ext::vector<T>::insert(
        const iterator where,
        args&&... item)
        -> vector&
{
    // insert the item in the middle of the veque, and return a reference to it
    LIVE_FILTER_MUTABLE(insert, where, std::forward<args>(item)...);
    this->m_iterable.emplace(this->begin() + where, std::forward<args>(item)...);
    return *this;
}


template <typename T>
auto ext::vector<T>::extend(
        const vector<T>& other,
        const iterator where)
        -> vector&
{
    // reverse the other list so that it can be inserted in the correct order ie 3 then 2 then 1 so {1, 2, 3} is added
    auto reversed_other = static_cast<ext::vector<T>>(other.reversed());
    reversed_other.for_each([this, other, where](const T& item) {insert(where, item);});

    // return a reference to the veque
    return *this;
}


template <typename T>
auto ext::vector<T>::pop(
        const iterator where)
        -> T&
{
    // remove an item based on its index - syntactic sugar for v.remove(v.at(...))
    LIVE_FILTER_MUTABLE(pop, where);
    // TODO
}


template <typename T>
auto ext::vector<T>::clear(
        ) -> vector&
{
    // clear the list and shrink it down to 0 size to free memory
    LIVE_FILTER_MUTABLE(clear);
    this->m_iterable.clear();
    this->m_iterable.shrink_to_fit();

    // return the reference to the veque
    return *this;
}


template <typename T>
auto ext::vector<T>::remove_all_before(
        const iterator where)
        -> vector&
{
    LIVE_FILTER_MUTABLE(remove_all_after, where);
    this->m_iterable.erase(this->begin(), where);
}


template <typename T>
auto ext::vector<T>::remove_all_after(
        const iterator where)
        -> vector&
{
    LIVE_FILTER_MUTABLE(remove_all_after, where);
    this->m_iterable.erase(where, this->end());
}


template <typename T>
auto ext::vector<T>::max_element(
        ) const -> T&
{
    // get the maximum element, and return the item at this iterator
    const auto where = std::max_element(this->begin(), this->end());
    return *where;
}


template <typename T>
auto ext::vector<T>::min_element(
        ) const -> T&
{
    // get the minimum element, and return the item at this iterator
    const auto where = std::min_element(this->begin(), this->end());
    return *where;
}


template <typename T>
template <typename F>
auto ext::vector<T>::all_of(
        F&& function) const
        -> bool
{
    // check if a condition matches all items in the veque, return early if 1 element doesn't match
    for (const T& item: *this)
    {
        if (not function(T{item})) return false;
    }
}


template <typename T>
template <typename F>
auto ext::vector<T>::any_of(
        F&& function) const
        -> bool
{
    // check if a condition matches any items in the veque, return early if 1 element does match
    for (const T& item: *this)
    {
        if (function(T{item})) return true;
    }
}


template <typename T>
template <typename F>
auto ext::vector<T>::for_each(
        F&& function)
        -> vector&
{
    // apply a function to each item in this veque, and return a reference to it
    LIVE_FILTER_MUTABLE(for_each, function);
    for (T item: this->m_iterable) function(item);
    return *this;
}


template <typename T>
template <typename F>
auto ext::vector<T>::for_each(
        F&& function) const
        -> const vector&
{
    // apply a const function to each item in this veque, and return a reference to it (for all_of, any_of etc...)
    for (const T item: this->m_iterable) function(item);
    return *this;
}


template <typename T>
template <typename F>
auto ext::vector<T>::filter(
        F&& function) const
        -> vector
{
    // remove non-matching items from a duplicate of the veque, and return it
    vector<T> copy{*this};
    copy.template remove_if<true>(&function);
    return difference(copy);
}


template <typename T>
template <typename U, typename F>
auto ext::vector<T>::transform(
        F&& function) const
        -> vector<U> // TODO : ext::vector<U>&& ret type?
{
    // create a duplicate of the veque, transform all the items in it, and return it
    vector<U> copy{};
    copy.reserve(this->length());
    std::transform(this->begin(), this->end(), std::back_inserter(copy), function);
    return copy;
}


template <typename T>
template <typename U>
auto ext::vector<T>::cast_all(
        ) const
        -> vector<U>
{
    // create a duplicate empty veque, and cast all the items from T to U
    vector<U> copy;
    constexpr bool is_dynamic = std::is_pointer_v<T> and std::is_pointer_v<U> and (std::is_base_of_v<T, U> or std::is_base_of_v<U, T>);

    if constexpr(is_dynamic)
        copy = transform<U>([](T item) {return dynamic_cast<U>(item);}).remove<true>(nullptr);
    else
        copy = transform<U>([](T& item) {return static_cast<U>(item);});

    return copy;
}


template <typename T>
auto ext::vector<T>::difference(
        vector& other) const
        -> vector
{
    auto difference_vector = ext::vector<T>{};
    difference_vector.reserve(this->length());
    std::set_difference(this->begin(), this->end(), other.begin(), other.end(), difference_vector.begin());
    difference_vector.shrink_to_fit();
    return difference_vector;
}


template <typename T>
auto ext::vector<T>::intersection(
        vector& other) const
        -> vector&
{
    // get the intersection by filtering this list based on if the other veque contains the item of not
    return filter(&other.contains);
}


template <typename T>
auto ext::vector<T>::join(
        char&& delimiter) const
        -> const char*
{
    // create a string and join each item in the veque to it
    std::string joined;
    for_each([&joined, delimiter](const T item) {joined += delimiter + item;});

    // return the const char( representation of the string
    return joined.c_str();
}


template <typename T>
template <typename U>
auto ext::vector<T>::flatten(
        ) -> ext::vector<U>
{
    /* TODO -> maybe use fold expression? */
}


template <typename T>
auto ext::vector<T>::call_all(
        ) const -> void requires std::is_invocable_v<T>
{
    // invoke each function in the veque
    for_each([](const T& item) {item();});
}


template <typename T>
template <typename F>
auto ext::vector<T>::attach_live_filtered(
        vector* live_iterable,
        F&& filter_lambda)
        -> void
{
    // attach the auto live filter, and set the flag for it
//    m_has_live_filtered_attachment_method = true;
//    m_live_filtered_attachment_method = std::forward<>(filter_lambda);
}


template <typename T>
auto ext::vector<T>::operator*(
        size_t n) const
        -> vector
{
    // create 2 copies of this veque (output and duplicator)
    auto original = vector{*this};
    auto output = vector{*this};

    // append flattened original to the output n times, and return it
    for (size_t i = 0; i < n; ++i) output.extend(original);
    return output;
}


template <typename iterator>
auto iterator_to_index(
        iterator iter)
        -> size_t
{
    // convert the iterator to an index, same thing as vector.index_of(*iterator)
    return std::distance(iter.begin(), iter);
}


template class ext::vector<int>;
template class ext::vector<A>;
template class ext::vector<A*>;



#endif //SBROWSER_VECTOR_HPP
