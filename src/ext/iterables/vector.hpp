#pragma once
#ifndef SBROWSER_VECTOR_HPP
#define SBROWSER_VECTOR_HPP

#include <string>

#include <ext/iterables/iterable.hpp>

#include <veque/include/veque.hpp>

namespace ext {
    template <typename T> class vector;
    template <typename T> using cvector = const vector<T>;
    class string;
}


namespace {
    template <class T, class = void>
    struct is_iterator : std::false_type{};

    template <class T>
    struct is_iterator<T, std::void_t<
            typename std::iterator_traits<T>::pointer,
            typename std::iterator_traits<T>::reference>>
            : std::true_type {
    };

    template <typename T>
    constexpr bool is_iterator_v = is_iterator<T>::value;
}


template <typename T>
class ext::vector : public ext::iterable<T, veque::veque<T>> {
public: constructors
    vector() = default;
    vector(const vector&) = default;
    vector(vector&&) noexcept = default;
    vector& operator=(const vector&) = default;
    vector& operator=(vector&&) noexcept = default;
    template <typename iterator> vector(iterator begin, iterator end) requires is_iterator_v<iterator>;
    template <typename ...args> vector(args&&... items);
    ~vector() override;

public: methods
    // element access
    T& item_before(const T& item) const;
    T& item_after(const T& item) const;
    template <typename function> T& first_match(function&& func) const;
    template <typename function> T& last_match(function&& func) const;
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
    template <typename function> bool all_of(function&& func) const;
    template <typename function> bool any_of(function&& func) const;
    template <typename function> vector<T>& for_each(function&& func);
    template <typename function> cvector<T>& for_each(function&& func) const;
    template <typename function> vector<T> filter(function&& func) const;
    template <typename U=T, typename function> vector<U> transform(function&& func) const;
    template <typename U=T> vector<U> cast_all() const;
    vector<T>& intersection(vector<T>& other) const;
    const char* join(char&& delimiter = ' ') const;
    vector<T>& flatten();
    void call_all() const requires std::is_invocable_v<T>;

public: operators
    ext::vector<T> operator*(size_t multiplier) const;
};


template <typename iterator>
size_t iterator_to_index(iterator iter) {
    return std::distance(iter.begin(), iter);
}


template <typename T>
template <typename iterator>
ext::vector<T>::vector(iterator begin, iterator end) requires is_iterator_v<iterator> {
    this->m_iterable = veque::veque<T>{begin, end};
}

template <typename T>
template <typename ...args> ext::vector<T>::vector(args&&... items) {
    this->m_iterable = veque::veque<T>{items...};
}

template <typename T>
ext::vector<T>::~vector() {
    this->m_iterable.clear();
    this->m_iterable.shrink_to_fit();
}

template <typename T>
inline T& ext::vector<T>::item_before(const T& item) const {
    return this->at((this->find(item) - 1) % this->length());
}

template <typename T>
inline T& ext::vector<T>::item_after(const T& item) const {
    return this->at((this->find(item) + 1) % this->length());
}

template <typename T>
template <typename function>
inline T& ext::vector<T>::first_match(function&& func) const {
    return filter(func).front();
}

template <typename T>
template <typename function>
inline T& ext::vector<T>::last_match(function&& func) const {
    return filter(func).back();
}

template <typename T>
inline ext::vector<T>& ext::vector<T>::slice(size_t front_index, size_t back_index) const {
    return vector<T>{this->begin() + front_index, this->begin() + back_index};
}

template <typename T>
inline ext::vector<T>& ext::vector<T>::append(const T& item) {
    this->m_iterable.emplace_back(item);
    return *this;
}

template <typename T>
inline ext::vector<T>& ext::vector<T>::prepend(const T& item) {
    this->m_iterable.emplace_front(this->begin(), item);
    return *this;
}

template <typename T>
inline ext::vector<T>& ext::vector<T>::insert(const T& item, const size_t index) {
    this->m_iterable.emplace(this->begin() + index, item);
    return *this;
}

template <typename T>
inline ext::vector<T>& ext::vector<T>::extend(const vector<T>& other, const size_t index) {
    auto reversed_other = reinterpret_cast<ext::vector<T>&>(other.reversed());
    reversed_other.for_each([this, other, index = index % this->length()](const T& item) -> void {insert(item, index);});
    return *this;
}

template <typename T>
inline ext::vector<T>& ext::vector<T>::pop(size_t index) {
    this->remove(this->at(index));
    return *this;
}

template <typename T>
inline ext::vector<T>& ext::vector<T>::clear() {
    this->m_iterable.clear();
    this->m_iterable.shrink_to_fit();
    return *this;
}

template <typename T>
inline T& ext::vector<T>::max_element() const {
    return this->at_iter(std::max_element(this->begin(), this->end()));
}

template <typename T>
inline T& ext::vector<T>::min_element() const {
    return this->at_iter(std::min_element(this->begin(), this->end()));
}

template <typename T>
template <typename function>
inline bool ext::vector<T>::all_of(function&& func) const {
    bool flag = true;
    for_each([&flag, func](const T& item) -> void {flag &= func(T{item});});
    return flag;
}

template <typename T>
template <typename function>
inline bool ext::vector<T>::any_of(function&& func) const {
    bool flag = false;
    for_each([&flag, func](const T& item) -> void {flag |= func(item); if (flag) return;});
    return flag;
}

template <typename T>
template <typename function>
inline ext::vector<T>& ext::vector<T>::for_each(function&& func) {
    for (T item: this->m_iterable) func(item);
    return *this;
}

template <typename T>
template <typename function>
inline ext::cvector<T>& ext::vector<T>::for_each(function&& func) const {
    for (const T item: this->m_iterable) func(item);
    return *this;
}

template <typename T>
template <typename function>
inline ext::vector<T> ext::vector<T>::filter(function&& func) const {
    return vector<T>{*this}.remove_if([func](const T& item) -> bool {return not func(item);});
}

template <typename T>
template <typename U, typename function>
inline ext::vector<U> ext::vector<T>::transform(function&& func) const {
    vector<U> copy{*this};
    std::transform(this->begin(), this->end(), copy.begin(), func);
    return copy;
}

template <typename T>
template <typename U>
inline ext::vector<U> ext::vector<T>::cast_all() const {
    vector<U> copy;
    copy = std::is_pointer_v<T>
           ? transform<U>([](const T& item) -> U {return dynamic_cast<U>(item);})
           : transform<U>([](const T& item) -> U {return static_cast<U>(item);});

    if (std::is_pointer_v<T>) copy.remove(nullptr, true);
    return copy;
}

template <typename T>
inline ext::vector<T>& ext::vector<T>::intersection(vector<T>& other) const {
    return filter(&other.contains);
}

template <typename T>
inline const char* ext::vector<T>::join(char&& delimiter) const {
    std::string joined;
    for_each([&joined, delimiter](const T item) -> void {joined += delimiter + item;});
    return joined.c_str();
}

template <typename T>
inline ext::vector<T>& ext::vector<T>::flatten() {
    /* TODO */
}

template <typename T>
inline void ext::vector<T>::call_all() const requires std::is_invocable_v<T> {
    for_each([](const T& item) -> void {item();});
}

template <typename T>
inline ext::vector<T> ext::vector<T>::operator*(size_t multiplier) const {
    auto original = vector<string>{*this};
    auto output = vector<string>{*this};

    for (size_t i = 0; i < multiplier; ++i) output.extend(original);
    return output;
}



#endif //SBROWSER_VECTOR_HPP
