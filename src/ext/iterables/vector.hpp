#pragma once
#ifndef SBROWSER_VECTOR_HPP
#define SBROWSER_VECTOR_HPP

#include <queue>
#include <stack>
#include <string>

#include <ext/iterables/iterable.hpp>

#include <veque/include/veque.hpp>

namespace ext
{
    template <typename T> class vector;
    template <typename T> using cvector = const vector<T>;
    class string;
}

namespace
{
    template <typename T, class = void> struct is_iterator : std::false_type{};
    template <typename T> struct is_iterator<T, std::void_t<typename std::iterator_traits<T>::pointer, typename std::iterator_traits<T>::reference>> : std::true_type {};
    template <typename T> constexpr bool is_iterator_v = is_iterator<T>::value;

    template <typename T, class = void> struct is_iterable : std::false_type {};
    template <typename T> struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end  (std::declval<T>()))>> : std::true_type {};
    template <typename T> constexpr bool is_iterable_v = is_iterable<T>::value;
}


template <typename T>
class ext::vector : public ext::iterable<T, veque::veque<T>>
{
public: constructors
    vector() = default;
    vector(const vector&) = default;
    vector(vector&&) noexcept = default;
    template <typename iterable_t> vector(const iterable_t& iterable_object) requires is_iterable_v<iterable_t>;
    template <typename iterator_t> vector(iterator_t begin, iterator_t end) requires is_iterator_v<iterator_t>;
    template <typename ...args> vector(args&&... items);

    auto operator=(const vector&) -> vector<T>& = default;
    auto operator=(vector&&) noexcept -> vector<T>& = default;
    auto operator=(const std::queue<T>& o) -> vector<T>&;
    auto operator=(const std::stack<T>& o) -> vector<T>&;

    ~vector() override;

public: methods
    auto slice(size_t front_index, size_t back_index) const -> vector<T>&;
    auto item_before(const T& item) const -> T&;
    auto item_after(const T& item) const -> T&;
    template <typename F> auto first_match(F&& function) const -> T&;
    template <typename F> auto last_match(F&& function) const -> T&;

    auto reserve(size_t count) -> void;
    auto append(const T& item) -> vector<T>&;
    auto prepend(const T& item) -> vector<T>&;
    auto insert(const T& item, size_t index = -1) -> vector<T>&;
    auto extend(const vector<T>& other, size_t index = -1) -> vector<T>&;
    auto pop(size_t index = -1) -> vector<T>&;
    auto clear() -> vector<T>& override;
    auto max_element() const -> T&;
    auto min_element() const -> T&;

    template <typename F> auto all_of(F&& function) const -> bool;
    template <typename F> auto any_of(F&& function) const -> bool;
    template <typename F> auto for_each(F&& function) -> vector<T>&;
    template <typename F> auto for_each(F&& function) const -> cvector<T>&;
    template <typename F> auto filter(F&& function) const -> vector<T>;
    template <typename U=T, typename F> auto transform(F&& function) const -> vector<U>;
    template <typename U=T> auto cast_all() const -> vector<U>;

    auto intersection(vector<T>& other) const -> vector<T>&;
    auto join(char&& delimiter = ' ') const -> const char*;
    auto flatten() -> vector<T>&;
    auto call_all() const -> void requires std::is_invocable_v<T>;

public: operators
    auto operator*(size_t n) const -> ext::vector<T>;
};


template <typename T>
template <typename iterable_t>
ext::vector<T>::vector(const iterable_t& iterable_object) requires is_iterable_v<iterable_t>
{
    // set the iterable to a veque from the iterator pair of the iterable_object
    this->m_iterable = veque::veque<T>{iterable_object.begin(), iterable_object.end()};
}


template <typename T>
template <typename iterator_t>
ext::vector<T>::vector(iterator_t begin, iterator_t end) requires is_iterator_v<iterator_t>
{
    // set the iterable to a veque defined by an iterator pair
    this->m_iterable = veque::veque<T>{begin, end};
}


template <typename T>
template <typename ...args> ext::vector<T>::vector(args&&... items)
{
    // set the iterable to a veque defined by a variable number of arguments
    this->m_iterable = veque::veque<T>{items...};
}


template <typename T>
auto ext::vector<T>::operator=(const std::queue<T>& o) -> ext::vector<T>&
{
    // fill the veque by emptying the queue
    auto copy = std::queue<T>{o};
    while (not copy.empty())
    {
        // append the front element and pop it from the queue
        append(copy.front());
        copy.pop();
    }
}


template <typename T>
auto ext::vector<T>::operator=(const std::stack<T>& o) -> ext::vector<T>&
{
    // fill the veque by emptying the stack
    auto copy = std::stack<T>{o};
    while (not copy.empty())
    {
        // append the top element and pop it from the queue
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
auto ext::vector<T>::slice(size_t front_index, size_t back_index) const -> ext::vector<T>&
{
    // slice the list by adding the front_index and back_index to the begin iterator
    return vector<T>{this->begin() + front_index, this->begin() + back_index};
}


template <typename T>
auto ext::vector<T>::item_before(const T& item) const -> T&
{
    // get the item in the veque found adjacent to the item passed in, before
    return this->at((this->find(item) - 1) % this->length());
}


template <typename T>
auto ext::vector<T>::item_after(const T& item) const -> T&
{
    // get the item in the veque found adjacent to the item passed in, after
    return this->at((this->find(item) + 1) % this->length());
}


template <typename T>
template <typename F>
auto ext::vector<T>::first_match(F&& function) const -> T&
{
    // get the first match by filtering the veque and returning the first item TODO : break on first item
    return filter(function).front();
}


template <typename T>
template <typename F>
auto ext::vector<T>::last_match(F&& function) const -> T&
{
    // get the last match by filtering the veque and returning the last match TODO : break on last item (reverse)
    return filter(function).back();
}


template <typename T>
auto ext::vector<T>::reserve(size_t count) -> void
{
    // reserve count amount of object space in the veque for quicker insertion
    this->m_iterable.reserve();
}


template <typename T>
auto ext::vector<T>::append(const T& item) -> ext::vector<T>&
{
    // append the item to the back of the veque, and return a reference to it
    this->m_iterable.emplace_back(item);
    return *this;
}


template <typename T>
auto ext::vector<T>::prepend(const T& item) -> ext::vector<T>&
{
    // prepend the item to the front of the veque, and return a reference to it
    this->m_iterable.emplace_front(this->begin(), item);
    return *this;
}


template <typename T>
auto ext::vector<T>::insert(const T& item, size_t index) -> ext::vector<T>&
{
    // insert the item in the middle of the veque, and return a reference to it
    this->m_iterable.emplace(this->begin() + index, item);
    return *this;
}


template <typename T>
auto ext::vector<T>::extend(const vector<T>& other, size_t index) -> ext::vector<T>&
{
    // reverse the other list so that it can be inserted in the correct order ie 3 then 2 then 1 so {1, 2, 3} is added
    auto reversed_other = other.reversed();
    reversed_other.for_each([this, other, index = index % this->length()](const T& item) -> void {insert(item, index);});

    // return a reference to the veque
    return *this;
}


template <typename T>
auto ext::vector<T>::pop(size_t index) -> ext::vector<T>&
{
    // remove an item based on its index - syntactic sugar for v.remove(v.at(...))
    this->remove(this->at(index));
    return *this;
}


template <typename T>
auto ext::vector<T>::clear() -> ext::vector<T>&
{
    // clear the list and shrink it down to 0 size to free memory
    this->m_iterable.clear();
    this->m_iterable.shrink_to_fit();

    // return the reference to the veque
    return *this;
}


template <typename T>
auto ext::vector<T>::max_element() const -> T&
{
    // get the maximum element, and return the item at this iterator
    return this->at_iter(std::max_element(this->begin(), this->end()));
}


template <typename T>
auto ext::vector<T>::min_element() const -> T&
{
    // get the minimum element, and return the item at this iterator
    return this->at_iter(std::min_element(this->begin(), this->end()));
}


template <typename T>
template <typename F>
auto ext::vector<T>::all_of(F&& function) const -> bool
{
    // set the flag to true by default
    bool flag = true;

    // check if a condition matches all items in the veque, break early if 1 element doesn't match
    for_each([&flag, function](const T& item) {
        flag &= function(T{item});
        if (not flag) return;
    });

    // return the modified flag
    return flag;
}


template <typename T>
template <typename F>
auto ext::vector<T>::any_of(F&& function) const -> bool
{
    // set the flag to false by default
    bool flag = false;

    // check if a condition matches any items in the veque, brake early if 1 element does match
    for_each([&flag, function](const T& item) {
        flag |= function(item);
        if (flag) return;
    });

    // return the modified flag
    return flag;
}


template <typename T>
template <typename F>
auto ext::vector<T>::for_each(F&& function) -> ext::vector<T>&
{
    // apply a function to each item in this veque, and return a reference to it
    for (T item: this->m_iterable) function(item);
    return *this;
}


template <typename T>
template <typename F>
auto ext::vector<T>::for_each(F&& function) const -> ext::cvector<T>&
{
    // apply a const function to each item in this veque, and return a reference to it (for all_of, any_of etc...)
    for (const T item: this->m_iterable) function(item);
    return *this;
}


template <typename T>
template <typename F>
auto ext::vector<T>::filter(F&& function) const -> ext::vector<T>
{
    // remove non-matching items from a duplicate of the veque, and return it
    return vector<T>{*this}.remove_if([function](const T& item) -> bool {return not function(item);});
}


template <typename T>
template <typename U, typename F>
auto ext::vector<T>::transform(F&& function) const -> ext::vector<U>
{
    // create a duplicate of the veque, transform all the items in it, and return a reference to it
    vector<U> copy{*this};
    std::transform(this->begin(), this->end(), copy.begin(), function);
    return copy;
}


template <typename T>
template <typename U>
auto ext::vector<T>::cast_all() const -> ext::vector<U>
{
    // create a duplicate empty veque, and cast all the items from T to U
    vector<U> copy;
    copy = std::is_pointer_v<T>
           ? transform<U>([](const T& item) -> U {return dynamic_cast<U>(item);})
           : transform<U>([](const T& item) -> U {return static_cast<U>(item);});

    // remove all empty elements from the copied list, and return a  reference to it
    if (std::is_pointer_v<T>) copy.clean(nullptr, true);
    return copy;
}


template <typename T>
auto ext::vector<T>::intersection(vector<T>& other) const -> ext::vector<T>&
{
    // get the intersection by filtering this list based on if the other veque contains the item of not
    return filter(&other.contains);
}


template <typename T>
auto ext::vector<T>::join(char&& delimiter) const -> const char*
{
    // create a string and join each item in the veque to it
    std::string joined;
    for_each([&joined, delimiter](const T item) -> void {joined += delimiter + item;});

    // return teh const char( representation of the string
    return joined.c_str();
}


template <typename T>
auto ext::vector<T>::flatten() -> ext::vector<T>&
{
    /* TODO */
}


template <typename T>
auto ext::vector<T>::call_all() const -> void requires std::is_invocable_v<T>
{
    // invoke each function in the veque
    for_each([](const T& item) {item();});
}


template <typename T>
auto ext::vector<T>::operator*(size_t n) const -> ext::vector<T>
{
    // create 2 copies of this veque (output and duplicator)
    auto original = vector<string>{*this};
    auto output = vector<string>{*this};

    // append flattened original to the output n times, and return it
    for (size_t i = 0; i < n; ++i) output.extend(original);
    return output;
}


template <typename iterator>
auto iterator_to_index(iterator iter) -> size_t
{
    // convert the iterator to an index, same thing as vector.index_of(*iterator)
    return std::distance(iter.begin(), iter);
}



#endif //SBROWSER_VECTOR_HPP
