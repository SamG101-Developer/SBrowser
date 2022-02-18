#pragma once
#ifndef SBROWSER_VECTOR_HPP
#define SBROWSER_VECTOR_HPP

#include <queue>
#include <stack>
#include <string>

#include <ext/iterables/iterable.hpp>

#include <veque/include/veque.hpp>

namespace ext {
    template <typename T> class vector;
    template <typename T> using cvector = const vector<T>;
    class string;
}

namespace {
    template <typename T, class = void> struct is_iterator : std::false_type{};
    template <typename T> struct is_iterator<T, std::void_t<typename std::iterator_traits<T>::pointer, typename std::iterator_traits<T>::reference>> : std::true_type {};
    template <typename T> constexpr bool is_iterator_v = is_iterator<T>::value;

    template <typename T, class = void> struct is_iterable : std::false_type {};
    template <typename T> struct is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end  (std::declval<T>()))>> : std::true_type {};
    template <typename T> constexpr bool is_iterable_v = is_iterable<T>::value;
}


template <typename T>
class ext::vector : public ext::iterable<T, veque::veque<T>> {
public: constructors
    vector() = default;
    vector(const vector&) = default;
    vector(vector&&) noexcept = default;
    vector<T>& operator=(const vector&) = default;
    vector<T>& operator=(vector&&) noexcept = default;
    vector<T>& operator=(const std::queue<T>& o);
    vector<T>& operator=(const std::stack<T>& o);
    template <typename iterator> vector(iterator begin, iterator end) requires is_iterator_v<iterator>;
    template <typename ...args> vector(args&&... items);
    ~vector() override;

public: methods
    // element access
    T& item_before(const T& item) const;
    T& item_after(const T& item) const;
    template <typename F> T& first_match(F&& function) const;
    template <typename F> T& last_match(F&& function) const;
    vector<T>& slice(size_t front_index, size_t back_index) const;

    // modifiers
    vector<T>& append(const T& item);
    vector<T>& prepend(const T& item);
    vector<T>& insert(const T& item, const size_t index = -1);
    vector<T>& extend(const vector<T>& other, const size_t index = -1);
    vector<T>& pop(size_t index = -1);
    vector<T>& clear() override;
    T& max_element() const;
    T& min_element() const;

    // algorithms
    template <typename F> bool all_of(F&& function) const;
    template <typename F> bool any_of(F&& function) const;
    template <typename F> vector<T>& for_each(F&& function);
    template <typename F> cvector<T>& for_each(F&& function) const;
    template <typename F> vector<T> filter(F&& function) const;
    template <typename U=T, typename F> vector<U> transform(F&& function) const;
    template <typename U=T> vector<U> cast_all() const;
    vector<T>& intersection(vector<T>& other) const;
    const char* join(char&& delimiter = ' ') const;
    vector<T>& flatten();
    void call_all() const requires std::is_invocable_v<T>;

public: operators
    ext::vector<T> operator*(size_t n) const;
};


template <typename T>
inline ext::vector<T>& ext::vector<T>::operator=(const std::queue<T>& o)
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
inline ext::vector<T>& ext::vector<T>::operator=(const std::stack<T>& o)
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
template <typename iterator>
ext::vector<T>::vector(iterator begin, iterator end) requires is_iterator_v<iterator>
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
ext::vector<T>::~vector()
{
    // call the clear method to clear up the list before it is deleted
    clear();
}


template <typename T>
inline T& ext::vector<T>::item_before(const T& item) const
{
    // get the item in the veque found adjacent to the item passed in, before
    return this->at((this->find(item) - 1) % this->length());
}


template <typename T>
inline T& ext::vector<T>::item_after(const T& item) const
{
    // get the item in the veque found adjacent to the item passed in, after
    return this->at((this->find(item) + 1) % this->length());
}


template <typename T>
template <typename F>
inline T& ext::vector<T>::first_match(F&& function) const
{
    // get the first match by filtering the veque and returning the first item TODO : break on first item
    return filter(function).front();
}


template <typename T>
template <typename F>
inline T& ext::vector<T>::last_match(F&& function) const
{
    // get the last match by filtering the veque and returning the last match TODO : break on last item (reverse)
    return filter(function).back();
}


template <typename T>
inline ext::vector<T>& ext::vector<T>::slice(size_t front_index, size_t back_index) const
{
    // slice the list by adding the front_index and back_index to the begin iterator
    return vector<T>{this->begin() + front_index, this->begin() + back_index};
}


template <typename T>
inline ext::vector<T>& ext::vector<T>::append(const T& item)
{
    // append the item to the back of the veque, and return a reference to it
    this->m_iterable.emplace_back(item);
    return *this;
}


template <typename T>
inline ext::vector<T>& ext::vector<T>::prepend(const T& item)
{
    // prepend the item to the front of the veque, and return a reference to it
    this->m_iterable.emplace_front(this->begin(), item);
    return *this;
}


template <typename T>
inline ext::vector<T>& ext::vector<T>::insert(const T& item, const size_t index)
{
    // insert the item in the middle of the veque, and return a reference to it
    this->m_iterable.emplace(this->begin() + index, item);
    return *this;
}


template <typename T>
inline ext::vector<T>& ext::vector<T>::extend(const vector<T>& other, const size_t index)
{
    // reverse the other list so that it can be inserted in the correct order ie 3 then 2 then 1 so {1, 2, 3} is added
    auto reversed_other = other.reversed();
    reversed_other.for_each([this, other, index = index % this->length()](const T& item) -> void {insert(item, index);});

    // return a reference to the veque
    return *this;
}


template <typename T>
inline ext::vector<T>& ext::vector<T>::pop(size_t index)
{
    // remove an item based on its index - syntactic sugar for v.remove(v.at(...))
    this->remove(this->at(index));
    return *this;
}


template <typename T>
inline ext::vector<T>& ext::vector<T>::clear()
{
    // clear the list and shrink it down to 0 size to free memory
    this->m_iterable.clear();
    this->m_iterable.shrink_to_fit();

    // return the reference to the veque
    return *this;
}


template <typename T>
inline T& ext::vector<T>::max_element() const
{
    // get the maximum element, and return the item at this iterator
    return this->at_iter(std::max_element(this->begin(), this->end()));
}


template <typename T>
inline T& ext::vector<T>::min_element() const
{
    // get the minimum element, and return the item at this iterator
    return this->at_iter(std::min_element(this->begin(), this->end()));
}


template <typename T>
template <typename F>
inline bool ext::vector<T>::all_of(F&& function) const
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
inline bool ext::vector<T>::any_of(F&& function) const
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
inline ext::vector<T>& ext::vector<T>::for_each(F&& function)
{
    // apply a function to each item in this veque, and return a reference to it
    for (T item: this->m_iterable) function(item);
    return *this;
}


template <typename T>
template <typename F>
inline ext::cvector<T>& ext::vector<T>::for_each(F&& function) const
{
    // apply a const function to each item in this veque, and return a reference to it (for all_of, any_of etc...)
    for (const T item: this->m_iterable) function(item);
    return *this;
}


template <typename T>
template <typename F>
inline ext::vector<T> ext::vector<T>::filter(F&& function) const
{
    // remove non-matching items from a duplicate of the veque, and return it
    return vector<T>{*this}.remove_if([function](const T& item) -> bool {return not function(item);});
}


template <typename T>
template <typename U, typename F>
inline ext::vector<U> ext::vector<T>::transform(F&& function) const
{
    // create a duplicate of the veque, transform all the items in it, and return a reference to it
    vector<U> copy{*this};
    std::transform(this->begin(), this->end(), copy.begin(), function);
    return copy;
}


template <typename T>
template <typename U>
inline ext::vector<U> ext::vector<T>::cast_all() const
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
inline ext::vector<T>& ext::vector<T>::intersection(vector<T>& other) const
{
    // get the intersection by filtering this list based on if the other veque contains the item of not
    return filter(&other.contains);
}


template <typename T>
inline const char* ext::vector<T>::join(char&& delimiter) const
{
    // create a string and join each item in the veque to it
    std::string joined;
    for_each([&joined, delimiter](const T item) -> void {joined += delimiter + item;});

    // return teh const char( representation of the string
    return joined.c_str();
}


template <typename T>
inline ext::vector<T>& ext::vector<T>::flatten()
{
    /* TODO */
}


template <typename T>
inline void ext::vector<T>::call_all() const requires std::is_invocable_v<T>
{
    // invoke each function in the veque
    for_each([](const T& item) {item();});
}


template <typename T>
inline ext::vector<T> ext::vector<T>::operator*(size_t n) const
{
    // create 2 copies of this veque (output and duplicator)
    auto original = vector<string>{*this};
    auto output = vector<string>{*this};

    // append flattened original to the output n times, and return it
    for (size_t i = 0; i < n; ++i) output.extend(original);
    return output;
}


template <typename iterator>
size_t iterator_to_index(iterator iter) {
    return std::distance(iter.begin(), iter);
}



#endif //SBROWSER_VECTOR_HPP
