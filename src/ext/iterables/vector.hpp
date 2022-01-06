#pragma once
#ifndef SBROWSER_VECTOR_HPP
#define SBROWSER_VECTOR_HPP

#include <deque>
#include <vector>
#include <string>

#include <ext/iterables/iterable.hpp>

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
class ext::vector : public ext::iterable<T, std::vector<T>> {
public: constructors
    vector() = default;
    vector(const vector&) = default;
    vector(vector&&) noexcept = default;
    vector& operator=(const vector&) = default;
    vector& operator=(vector&&) noexcept = default;

    template <typename iterator>
    vector(iterator begin, iterator end) requires (is_iterator_v<iterator>) {
        this->m_iterable = std::vector<T>{begin, end};
    }

    template <typename ...args> vector(args&&... items) {
        this->m_iterable = std::vector<T>{items...};
    }

    ~vector() override {
        this->m_iterable.clear();
        this->m_iterable.shrink_to_fit();
    }

public: methods
    // element access
    inline T& item_before(const T& item) {
        return this->at((this->find(item) - 1) % this->length());
    }

    inline T& item_after(const T& item) {
        return this->at((this->find(item) + 1) % this->length());
    }

    template <typename function> inline T& first_match(function&& func) {
        return filter(func).front();
    }

    template <typename function> inline T& last_match(function&& func) {
        return filter(func).back();
    }

    inline vector<T>& slice(size_t front_index, size_t back_index) const {
        return vector<T>{this->begin() + front_index, this->begin() + back_index};
    }

    // modifiers
    inline vector<T>& append(const T& item) {
        this->m_iterable.emplace_back(item);
        return *this;
    }

    inline vector<T>& prepend(const T& item) {
        this->m_iterable.emplace(this->begin(), item);
        return *this;
    }

    inline vector<T>& insert(const T& item, const size_t index) {
        this->m_iterable.emplace(this->begin() + index, item);
        return *this;
    }

    inline vector<T>& extend(const vector<T>& other, const size_t index = -1) {
        auto reversed_other = reinterpret_cast<ext::vector<T>&>(other.reversed());
        reversed_other.for_each([this, other, index = index % this->length()](const T& item) -> void {insert(item, index);});
        return *this;
    }

    inline vector<T>& pop(size_t index = -1) {
        this->remove(this->at(index));
        return *this;
    }

    inline vector<T>& clear() override {
        ext::iterable<T, std::vector<T>>::clear();
        this->m_iterable.shrink_to_fit();
        return *this;
    }

    inline T& max_element() {
        return this->at_iter(std::max_element(this->begin(), this->end()));
    }

    // algorithms
    template <typename function> inline bool all_of(function&& func) const {
        bool flag = true;
        for_each([&flag, func](const T& item) -> void {flag &= func(T{item});});
        return flag;
    }

    template <typename function> inline bool any_of(function&& func) const {
        bool flag = false;
        for_each([&flag, func](const T& item) -> void {flag |= (bool)func(T{item}); if (flag) return;});
        return flag;
    }

    template <typename function> inline vector<T>& for_each(function&& func) {
        for (T item: this->m_iterable) func(item);
        return *this;
    }

    template <typename function> inline vector<T> filter(function&& func) const {
        return vector<T>{*this}.remove_if([func](const T& item) -> bool {return not func(item);});
    }

    template <typename U=T, typename function> inline vector<U> transform(function&& func) const {
        vector<U> copy{*this};
        std::transform(this->begin(), this->end(), copy.begin(), func);
        return copy;
    }

    template <typename U> inline vector<U> cast_all() {
        vector<U> copy;
        copy = std::is_pointer_v<T>
                ? transform<U>([](const T& item) -> U {return dynamic_cast<U>(item);})
                : transform<U>([](const T& item) -> U {return static_cast<U>(item);});

        if (std::is_pointer_v<T>) copy.remove(nullptr, true);
        return copy;
    }

    inline vector<T>& intersection(vector<T>& other) {
        return filter(&other.contains);
    }

    inline std::string join(char delimiter = ' ') {
        std::string joined;
        for_each([&joined, delimiter](const T item) -> void {joined += delimiter + item;});
        return joined;
    }

    inline vector<T>& flatten() {/* TODO */}

public: operators
    inline ext::vector<T> operator*(size_t multiplier) const {
        auto original = vector<string>{*this};
        auto output = vector<string>{*this};

        for (size_t i = 0; i < multiplier; ++i) output.extend(original);
        return output;
    }
};


template <typename iterator>
size_t iterator_to_index(iterator iter) {
    return std::distance(iter.begin(), iter);
}


#endif //SBROWSER_VECTOR_HPP
