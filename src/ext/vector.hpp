#pragma once
#ifndef SBROWSER_VECTOR_HPP
#define SBROWSER_VECTOR_HPP

#include <deque>
#include <vector>
#include <string>

#include <ext/iterable.hpp>

namespace ext {
    template <typename T> class vector;
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
public:
    vector() = default;
    vector(const vector&) = default;
    vector(vector&&) noexcept = default;
    vector& operator=(const vector&) = default;
    vector& operator=(vector&&) noexcept = default;

    template <typename iterator> vector(iterator begin, iterator end) requires (is_iterator_v<iterator>) {this->m_iterable = std::vector<T>{begin, end};}

    template <typename ...args> vector(args&&... items) {this->m_iterable = std::vector<T>{items...};}

    virtual ~vector() {
        this->m_iterable.clear();
        this->m_iterable.shrink_to_fit();
    }

    inline T& item_before(const T& item) {return at((find(item) - 1) % this->length());}
    inline T& item_after (const T& item) {return at((find(item) + 1) % this->length());}

    inline vector<T>& append(const T& item) {
        this->m_iterable.emplace_back(item);
        return *this;
    }

    inline vector<T>& prepend(const T& item) {
        this->m_iterable.emplace(this->begin(), item);
        return *this;
    }

    inline vector<T>& insert(const T& item, const std::size_t index) {
        this->m_iterable.emplace(this->begin() + index, item);
        return *this;
    }

    inline vector<T>& extend(const vector<T>& other, const std::size_t index = -1) {
        // other.reversed().for_each([this, other, index = index % this->length()](T item) -> void {insert(other, index);});
        return *this;
    }

    inline vector<T>& pop(std::size_t index = -1) {
        remove(this->at(index));
        return *this;
    }

    template <typename function> inline bool all_of(function&& func) const {
        bool flag = true;
        for_each([&flag, func](const T& item) -> void {flag &= func(T{item});});
        return flag;
    }

    template <typename function> inline bool any_of(function&& func) const {
        bool flag = false;
        for_each([&flag, func](const T& item) -> void {flag |= func(T{item}); if (flag) return;});
        return flag;
    }

    template <typename function> inline vector<T>& for_each(function&& func) {
        for (T item: this->m_iterable) func(item);
        return *this;
    }

    template <typename function> inline vector<T>& filter(function func) const {
        return vector<T>{*this}.remove_if([func](const T& item) -> void {return not func(item);});
    }

    template <typename U=T, typename function> inline vector<U> transform(function func) const {
        vector<U> copy{*this};
        std::transform(this->begin(), this->end(), copy.begin(), func);
        return copy;
    }

    template <typename U> inline vector<U> cast_all() {
        vector<U> copy;
        copy = std::is_pointer_v<T>
                ? &transform<U>([](const T& item) -> U {return dynamic_cast<U>(item);})
                : &transform<U>([](const T& item) -> U {return static_cast<U>(item); });

        if (std::is_pointer_v<T>) copy.remove(nullptr, true);
        return copy;
    }

    inline vector<T>& intersection(vector<T>& other) {
        return filter(&other.contains);
    }

    inline std::string join(std::string delimiter = "") {
        std::string joined;
        // for_each([&joined, delimiter](const T item) -> void {joined += delimiter + item;});
        return joined;
    }

    inline ext::vector<T> operator*(std::size_t multiplier) const {
        auto original = vector<string>{*this};
        auto output   = vector<string>{*this};

        for (std::size_t i = 0; i < multiplier; ++i) output.extend(original);
        return output;
    }
};


template <typename iterator>
std::size_t iterator_to_index(iterator iter) {
    return std::distance(iter.begin(), iter);
}


#endif //SBROWSER_VECTOR_HPP
