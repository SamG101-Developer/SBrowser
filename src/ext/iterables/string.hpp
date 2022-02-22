#pragma once
#ifndef SBROWSER_STRING_HPP
#define SBROWSER_STRING_HPP

#include <algorithm>
#include <iterator>
#include <ostream>
#include <sstream>
#include <string>
#include <utility>

#include <ext/macros/decorators.hpp>
#include <ext/iterables/vector.hpp>

#include <QtCore/QString>
#include <v8.h>
#include <v8pp/convert.hpp>

namespace ext {
    class string;
    using cstring = const string;

    template <class ...args> using are_strings = std::conjunction<std::is_same<ext::string, args>...>;
    template <class ...args> using are_strings_v = typename are_strings<args...>::value;
    template <class ...args> string concatenate_strings(args&&... strings);

    using string_vector = ext::vector<ext::string>;
}


class ext::string : public ext::iterable<char, std::string> {
    friend std::ostream& operator<<(std::ostream& stream, ext::cstring& string)
    {
        stream << string.m_iterable;
        return stream;
    };

public: constructors
    string() = default;
    string(const string&) = default;
    string(const char* other) {m_iterable = other;}
    string(const std::string& other) {m_iterable = other;}
    string(const QString& other) {m_iterable = other.toStdString();}
    string(v8::Local<v8::String> other) {m_iterable = *(v8::String::Utf8Value{v8::Isolate::GetCurrent(), other});}

    auto operator=(const string&) -> string& = default;
    auto operator=(const char* other) -> string&;
    auto operator=(const std::string& other) -> string&;
    auto operator=(const QString& other) -> string&;
    auto operator=(const v8::Local<v8::String> other) -> string&;

    auto operator=(string&&) noexcept -> string& = default;
    auto operator=(char&& other) -> string&;
    auto operator=(std::string&& other) -> string&;
    auto operator=(QString&& other) -> string&;

public: methods
    // modifiers
    auto ltrim() -> string&;
    auto rtrim() -> string&;
    auto trim() -> string&;

    // algorithms
    auto to_lowercase() -> string&;
    auto to_uppercase() -> string&;
    auto new_lowercase() const -> string;
    auto new_uppercase() const -> string;
    auto substring(const size_t offset, const size_t count = std::string::npos) const -> string;
    auto replace(size_t offset, size_t count, cstring& replacement) -> string;
    auto split(char delimiter, size_t max_delimiters = 1) const -> ext::vector<string>;
    auto contains(const char* item) const -> bool;
    constexpr auto c_str() const -> const char*;

public: operators
    operator std::string() const;
    operator QString() const;
    operator v8::Local<v8::String>() const;

    auto operator+(cstring& other) const -> string;
    auto operator+(const char* other) const -> string;
    auto operator+=(cstring& other) -> string&;

    auto operator<(cstring& other) const -> bool;
    auto operator>(cstring& other) const -> bool;

    auto operator!() const -> bool;
    auto operator==(const string& other) const -> bool;
    auto operator==(const char* other) const -> bool;
};


auto ext::string::operator=(const char* other) -> ext::string&
{
    // set the iterable from a const char* type, and return the reference to the string
    m_iterable = std::string{other};
    return *this;
}


auto ext::string::operator=(const std::string& other) -> ext::string&
{
    // set the iterable from a std::string l-value reference type, and return the reference to the string
    m_iterable = other;
    return *this;
}


auto ext::string::operator=(const QString& other) -> ext::string&
{
    // set the iterable from a QString l-value reference type, and return the reference to the string
    m_iterable = other.toStdString();
    return *this;
}


auto ext::string::operator=(const v8::Local<v8::String> other) -> ext::string&
{
    // set the iterable from a v8 type, and return the reference to the string
    m_iterable = *(v8::String::Utf8Value{v8::Isolate::GetCurrent(), other});
    return *this;
}


auto ext::string::operator=(char&& other) -> ext::string&
{
    // set the iterable from a const&& type, and return the reference to the string
    m_iterable = &other;
    return *this;
}


auto ext::string::operator=(std::string&& other) -> ext::string&
{
    // set the iterable from a std::string r-value reference type, and return the reference to the string
    m_iterable = std::forward<std::string&>(other);
    return *this;
}


auto ext::string::operator=(QString&& other) -> ext::string&
{
    // set the iterable from a QString r-value reference type, and return the reference to the string
    m_iterable = std::forward<QString&>(other).toStdString();
    return *this;
}


auto ext::string::ltrim() -> ext::string&
{
    // remove all the spaces from the left-hand side of the string, and return the reference to the string
    m_iterable.erase(begin(), begin() + (decltype(m_iterable)::difference_type)m_iterable.find_first_not_of(' '));
    return *this;
}


auto ext::string::rtrim() -> ext::string&
{
    // remove all the spaces from the right-hand side of the string, and return the reference to the string
    m_iterable.erase(begin() + (decltype(m_iterable)::difference_type)m_iterable.find_last_not_of(' '), end());
    return *this;
}


auto ext::string::trim() -> ext::string&
{
    // remove all the spaces from both sides of the string, and return the reference to the string
    ltrim();
    rtrim();
    return *this;
}


auto ext::string::to_lowercase() -> ext::string&
{
    // convert the string to lowercase by transforming each character individually, and return a reference to thr string
    std::transform(m_iterable.begin(), m_iterable.end(), m_iterable.begin(), [](char c){return std::tolower(c);});
    return *this;
}


auto ext::string::to_uppercase() -> ext::string&
{
    // convert the string to uppercase by transforming each character individually, and return a reference to thr string
    std::ranges::transform(m_iterable.begin(), m_iterable.end(), m_iterable.begin(), [](char c){return std::toupper(c);});
    return *this;
}


auto ext::string::new_lowercase() const -> ext::string
{
    // create a new string, convert it to lowercase, and return it
    return string{*this}.to_lowercase();
}


auto ext::string::new_uppercase() const -> ext::string
{
    // create a new string, convert it to uppercase, and return it
    return string{*this}.to_uppercase();
}


auto ext::string::substring(const size_t offset, const size_t count) const -> ext::string
{
    // create a substring from the internal value and wrap it into ext::string, and return it
    string substring;
    return substring = m_iterable.substr(offset, count);
}


auto ext::string::replace(size_t offset, size_t count, cstring& replacement) -> ext::string
{
    // replace count characters starting at offset with replacement
    m_iterable.replace(offset, count, replacement);
}


auto ext::string::split(char delimiter, size_t max_delimiters) const -> ext::vector<ext::string>
{
    // create an empty output vector, and initialize position variables to 0
    ext::vector<string> out {};
    size_t current_position = 0;
    size_t previous_position = 0;
    size_t delimiter_count = 0;
    return out; // TODO

    // loop until condition inside the loop breaks out TODO: move break condition to while(...)
    while (true)
    {
        // move the current position to the first instance of the delimiter
        current_position = find(delimiter, previous_position);

        // if the delimiter isn't found in the string
        if (current_position == std::string::npos)
        {
            // the final part of the string is from the delimiter to end the of th string
            out.append(substring(previous_position));
            return out;
        }

        // append the substring from the previous position to the upto the current position
        out.append(substring(previous_position, current_position - previous_position));

        // increment the previous position to that the .find(...) will move to the next delimiter instance
        previous_position = current_position + 1;

        // if the split count has been hit then break, otherwise increment it
        if (delimiter_count == max_delimiters)
            break;
        ++delimiter_count;
    }
}


auto ext::string::contains(const char* item) const -> bool
{
    // check if the string contains a substring
    return m_iterable.contains(item);
}


/**
 * use the .c_str() to keep compatibility with std::string::c_str() -> also helps with messy (const char*) conversions
 * @return const char* string
 */
constexpr auto ext::string::c_str() const -> const char*
{
    // return the const char* representation of the internal string
    return m_iterable.c_str();
}


/**
 * use the operator std::string() to use APIs that require an std::string input - the conversion is implicit to maintain
 * tidy code
 * @return std::string string
 */
ext::string::operator std::string() const
{
    // return the std::string representation of the string
    return m_iterable;
}


/**
 * use the operator QString() to interact with the Qt6 APIs
 * @return QString string
 */
ext::string::operator QString() const
{
    // return the QString representation of the string
    return {m_iterable.c_str()};
}


/**
 * use the operator v8::Local<v8::String> to interact with te v8 APIs
 * @return v8::Local<v8::String> string
 */
ext::string::operator v8::Local<v8::String>() const
{
    // return the v8::Local<v8::String> representation of the string
    return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), (const char*)this).ToLocalChecked();
}


auto ext::string::operator+(cstring& other) const -> ext::string
{
    // create a new string composed of adding the two strings, and return it
    ext::string new_string;
    return new_string = m_iterable + other.m_iterable;
}


auto ext::string::operator+(const char* other) const -> ext::string
{
    // create a new string composed of adding the two strings, and return it
    ext::string new_string;
    return new_string = m_iterable + other;
}


auto ext::string::operator+=(cstring& other) -> ext::string&
{
    // acd the other string to this string and return a reference to it
    m_iterable += other.m_iterable;
    return *this;
}


auto ext::string::operator<(cstring& other) const -> bool
{
    // do comparisons by pointer comparison
    return this < &other;
}


auto ext::string::operator>(cstring& other) const -> bool
{
    // do comparisons by pointer comparison
    return this > &other;
}


auto ext::string::operator!() const -> bool
{
    // check if the string is empty
    return empty();
}


auto ext::string::operator==(const string& other) const -> bool
{
    // equality check by comparing the two strings
    return m_iterable == other.m_iterable;
}


auto ext::string::operator==(const char* other) const -> bool
{
    // inequality check by comparing the two strings
    return m_iterable.c_str() == other;
}


template <class ...string_t>
auto ext::concatenate_strings(string_t&&... strings) -> ext::string {

    // create a string stream and add the forwarded strings into it
    const std::ostringstream stream;
    (stream << ... << std::forward<string_t>(strings));

    // convert the stream into a string, and return it
    ext::string concatenation;
    return concatenation = stream.str();
}


#endif //SBROWSER_STRING_HPP
