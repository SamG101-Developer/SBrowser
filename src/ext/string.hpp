#pragma once
#ifndef SBROWSER_STRING_HPP
#define SBROWSER_STRING_HPP

#include <algorithm>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include <ext/vector.hpp>

#include <QtCore/QString>
#include <v8.h>

const ext::vector<std::string> STRING_BOOLEANS{"true", "false", "1", "0"};
const ext::vector<std::string> STRING_ALPHAS  {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"};
const ext::vector<std::string> STRING_HEX     {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};

namespace ext {
    class string;
    using cstring = const ext::string;

    template <class ...args> using are_strings = std::conjunction<std::is_same<ext::string, args>...>;
    template <class ...args> using are_strings_v = typename are_strings<args...>::value;

    template <class ...args> string concatenate_strings(args&&... strings);
}


class ext::string: public ext::iterable<char, std::string> {
public friends:
    friend std::ostream& operator<<(std::ostream& stream, ext::cstring& string) {
        stream << string.m_iterable;
        return stream;
    };

public constructors:
    string() = default;
    string(const string&) = default;
    string(string&&) noexcept = default;
    ext::string& operator = (const string&) = default;
    ext::string& operator = (string&&) noexcept = default;

    string(const std::string& other) {m_iterable = other;}; // TODO
    string(std::string&& other) noexcept {m_iterable = std::forward<std::string&>(other);}

    string(const char* other) {m_iterable = (std::string)other;}

    string(const QString& other) {m_iterable = other.toStdString();}
    string(QString&& other) {m_iterable = std::forward<QString&>(other).toStdString();}

public methods:
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

    inline ext::string substring(const std::size_t offset, const std::size_t count = std::string::npos) const {
        return ext::string{m_iterable.substr(offset, count)};
    }

    inline ext::string& ltrim() {
        m_iterable.erase(begin(), begin() + m_iterable.find_first_not_of(" "));
        return *this;
    }

    inline ext::string& rtrim() {
        m_iterable.erase(begin() + m_iterable.find_last_not_of(" ") + 1, end());
        return *this;
    }

    inline ext::string& trim() {
        ltrim(); rtrim();
        return *this;
    }

    inline ext::vector<ext::string> split(ext::string&& delimiter) const {
        ext::vector<ext::string> out {};
        std::size_t current_position = 0;
        std::size_t previous_position = 0;

        while (true) {
            current_position = find(*delimiter.to_string_const_char_p(), previous_position);
            if (current_position == std::string::npos) {
                out.append(substring(previous_position));
                return out;
            }

            out.append(substring(previous_position, current_position - previous_position));
            previous_position = current_position + delimiter.length();
        }
    }

    inline bool is_double() const {return std::all_of(begin(), end(), [](char c) -> bool {return ::isdigit(c) or c == *".";});}
    inline bool is_integer() const {return std::all_of(begin(), end(), [](char c) -> bool {return ::isdigit(c);});}
    inline bool is_bool() const {return std::all_of(m_iterable.begin(), m_iterable.end(), [](auto character) {return STRING_BOOLEANS.contains(std::string{character});});}
    inline bool is_alpha() const {return std::all_of(m_iterable.begin(), m_iterable.end(), [](auto character) {return STRING_ALPHAS.contains(std::string{character});});}
    inline bool is_hex() const {return std::all_of(m_iterable.begin(), m_iterable.end(), [](auto character) {return STRING_HEX.contains(std::string{character});});}

    inline double to_double() const {return std::stod(m_iterable);}
    inline long long to_integer() const {return std::stoll(m_iterable);}
    inline bool to_bool() const {return (bool)std::stoi(m_iterable);}


    inline std::string to_string_std() const {
        return m_iterable;
    }

    inline const char* to_string_const_char_p() const {
        return m_iterable.c_str();
    }

    inline QString to_string_qt() const {
        return {m_iterable.c_str()};
    }

    inline v8::Local<v8::String> to_string_v8() const {
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), m_iterable.c_str()).ToLocalChecked();
    }

    inline ext::string operator+(ext::cstring& other) const {return ext::string{m_iterable + other.m_iterable};}
    inline ext::string& operator+=(ext::cstring& other) {m_iterable += other.m_iterable; return *this;}

    inline bool operator<(ext::cstring& other) const {return this < &other;}
    inline bool operator>(ext::cstring& other) const {return this > &other;}

    inline bool operator!() const {return empty();}

    inline bool operator==(const std::string& other) const {return m_iterable == other;}
};


template <class ...args>
ext::string ext::concatenate_strings(args&&... strings) {

    std::ostringstream stream;
    (stream << ... << std::forward<args>(strings));
    return ext::string{stream.str()};
}


#endif //SBROWSER_STRING_HPP
