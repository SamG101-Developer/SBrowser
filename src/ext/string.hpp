#pragma once
#ifndef SBROWSER_STRING_HPP
#define SBROWSER_STRING_HPP

#include <algorithm>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include <ext/decorators.hpp>
#include <ext/vector.hpp>

#include <QtCore/QString>
#include <v8.h>
#include <v8pp/convert.hpp>

//static ext::vector<std::string> get_STRING_BOOLEANS() {return {"true", "false", "1", "0"};}
//static ext::vector<std::string> get_STRING_ALPHAS() {return {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};}
//static ext::vector<std::string> get_STRING_HEX() {return {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};}

namespace ext {
    class string;
    using cstring = const string;

    template <class ...args> using are_strings = std::conjunction<std::is_same<ext::string, args>...>;
    template <class ...args> using are_strings_v = typename are_strings<args...>::value;

    template <class ...args> string concatenate_strings(args&&... strings);
}


class ext::string : public ext::iterable<char, std::string> {
    friend std::ostream& operator<<(std::ostream& stream, ext::cstring& string) {
        stream << string.m_iterable;
        return stream;
    };

public: constructors
    string() = default;
    string(const string&) = default;
    string(string&&) noexcept = default;
    string& operator=(const string&) = default;
    string& operator=(string&&) noexcept = default;

    // std::string ctors
    string& operator=(const std::string& other) {
        m_iterable = other;
        return *this;
    }

    string& operator=(std::string&& other) {
        m_iterable = std::forward<std::string&>(other);
        return *this;
    }

    // QString ctors
    string& operator=(const QString& other) {
        m_iterable = other.toStdString();
        return *this;
    }

    string& operator=(QString&& other) {
        m_iterable = std::forward<QString&>(other).toStdString();
        return *this;
    }

    // char ctors
    string& operator=(const char* other) {
        m_iterable = std::string{other};
        return *this;
    }

    string& operator=(char&& other) {
        m_iterable = other;
        return *this;
    }

    // v8 ctors
    string& operator=(v8::Local<v8::String> other) {
        m_iterable = *(v8::String::Utf8Value{v8::Isolate::GetCurrent(), other});
        return *this;
    }

public: methods
    inline ext::string& to_lowercase() {
        std::ranges::transform(m_iterable.begin(), m_iterable.end(), m_iterable.begin(), [](char c){return std::tolower(c);});
        return *this;
    }

    inline ext::string& to_uppercase() {
        std::string hello{" "};
        std::ranges::transform(m_iterable.begin(), m_iterable.end(), m_iterable.begin(), [](char c){return std::toupper(c);});
        return *this;
    }

    inline ext::string& new_lowercase() const {
        return ext::string{*this}.to_lowercase();
    }

    inline ext::string& new_uppercase() const {
        return ext::string{*this}.to_uppercase();
    }

    inline ext::string substring(const size_t offset, const size_t count = std::string::npos) const {
        ext::string sub_string;
        sub_string = m_iterable.substr(offset, count);
        return sub_string;
    }

    inline ext::string& ltrim() {
        m_iterable.erase(begin(), begin() + (std::string::difference_type)m_iterable.find_first_not_of(" "));
        return *this;
    }

    inline ext::string& rtrim() {
        m_iterable.erase(begin() + (std::string::difference_type)m_iterable.find_last_not_of(" ") + 1, end());
        return *this;
    }

    inline ext::string& trim() {
        ltrim(); rtrim();
        return *this;
    }

    inline ext::vector<ext::string> split(char delimiter) const {
        ext::vector<ext::string> out {};
        size_t current_position = 0;
        size_t previous_position = 0;
        return out; // TODO

        while (true) {
            current_position = find(delimiter, previous_position);
            if (current_position == std::string::npos) {
                out.append(substring(previous_position));
                return out;
            }

            out.append(substring(previous_position, current_position - previous_position));
            previous_position = current_position + 1;
        }
    }
//
//    inline bool is_double() const {
//        return std::all_of(begin(), end(), [](char c) -> bool {return ::isdigit(c) or c == '.';});
//    }
//
//    inline bool is_integer() const {
//        return std::all_of(begin(), end(), [](char c) -> bool {return ::isdigit(c);});
//    }
//
//    inline bool is_bool() const {
//        return std::all_of(m_iterable.begin(), m_iterable.end(), [](auto character) {return get_STRING_BOOLEANS().contains(std::string{character});});
//    }
//
//    inline bool is_alpha() const {
//        return std::all_of(m_iterable.begin(), m_iterable.end(), [](auto character) {return get_STRING_ALPHAS().contains(std::string{character});});
//    }
//
//    inline bool is_hex() const {
//        return std::all_of(m_iterable.begin(), m_iterable.end(), [](auto character) {return get_STRING_HEX().contains(std::string{character});});
//    }

    inline double to_double() const {
        return std::stod(m_iterable);
    }

    inline int to_integer() const {
        return std::stoi(m_iterable);
    }

    inline std::string to_std_string() const {
        return m_iterable;
    }

    inline const char* to_c_str() const {
        return m_iterable.c_str();
    }

    inline QString to_string_qt() const {
        return {m_iterable.c_str()};
    }

    inline v8::Local<v8::String> to_string_v8() const {
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), to_c_str()).ToLocalChecked();
    }

public: operators
    inline ext::string operator+(ext::cstring& other) const {
        ext::string new_string;
        return new_string = m_iterable + other.m_iterable;
    }

    inline ext::string& operator+=(ext::cstring& other) {
        m_iterable += other.m_iterable;
        return *this;
    }

    inline bool operator<(ext::cstring& other) const {
        return this < &other;
    }

    inline bool operator>(ext::cstring& other) const {
        return this > &other;
    }

    inline bool operator!() const {
        return empty();
    }

    inline bool operator==(const ext::string& other) const {
        return m_iterable == other.m_iterable;
    }

    inline bool operator==(ext::string&& other) const {
        return m_iterable == other.m_iterable;
    }
};


template <class ...string_t>
ext::string ext::concatenate_strings(string_t&&... strings) {

    const std::ostringstream stream;
    (stream << ... << std::forward<string_t>(strings));
    ext::string concatenation;
    return concatenation = stream.str();
}


#endif //SBROWSER_STRING_HPP
