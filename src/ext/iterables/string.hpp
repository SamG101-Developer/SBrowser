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
    string(const char* other) {m_iterable = other;}

    string& operator=(const string&) = default;
    string& operator=(string&&) noexcept = default;

    string& operator=(const char* other);
    string& operator=(char&& other);

    string& operator=(const std::string& other);
    string& operator=(std::string&& other);

    string& operator=(const QString& other);
    string& operator=(QString&& other);

    string& operator=(v8::Local<v8::String> other);


public: methods
    // modifiers
    string& ltrim();
    string& rtrim();
    string& trim();

    // algorithms
    string& to_lowercase();
    string& to_uppercase();
    string& new_lowercase() const;
    string& new_uppercase() const;
    string substring(const size_t offset, const size_t count = std::string::npos) const;
    string replace(size_t offset, size_t count, cstring& replacement);
    ext::vector<string> split(char delimiter, size_t max_delimiters = 1) const;
    bool contains(const char* item) const;
    constexpr const char* c_str() const;

public: operators
    operator std::string() const;
    operator QString() const;
    operator v8::Local<v8::String>() const;

    string operator+(cstring& other) const;
    string operator+(const char* other) const;
    string& operator+=(cstring& other);

    bool operator<(cstring& other) const;
    bool operator>(cstring& other) const;

    bool operator!() const;
    bool operator==(const string& other) const;
    bool operator==(const char* other) const;
};


ext::string& ext::string::operator=(const char* other)
{
    // set the iterable from a const char* type, and return the reference to the string
    m_iterable = std::string{other};
    return *this;
}


ext::string& ext::string::operator=(char&& other)
{
    // set the iterable from a const&& type, and return the reference to the string
    m_iterable = &other;
    return *this;
}


ext::string& ext::string::operator=(const std::string& other)
{
    // set the iterable from a std::string l-value reference type, and return the reference to the string
    m_iterable = other;
    return *this;
}


ext::string& ext::string::operator=(std::string&& other)
{
    // set the iterable from a std::string r-value reference type, and return the reference to the string
    m_iterable = std::forward<std::string&>(other);
    return *this;
}


ext::string& ext::string::operator=(const QString& other)
{
    // set the iterable from a QString l-value reference type, and return the reference to the string
    m_iterable = other.toStdString();
    return *this;
}


ext::string& ext::string::operator=(QString&& other)
{
    // set the iterable from a QString r-value reference type, and return the reference to the string
    m_iterable = std::forward<QString&>(other).toStdString();
    return *this;
}


ext::string& ext::string::operator=(v8::Local<v8::String> other)
{
    // set the iterable from a v8 type, and return the reference to the string
    m_iterable = *(v8::String::Utf8Value{v8::Isolate::GetCurrent(), other});
    return *this;
}


inline ext::string& ext::string::ltrim()
{
    // remove all the spaces from the left-hand side of the string, and return the reference to the string
    m_iterable.erase(begin(), begin() + (decltype(m_iterable)::difference_type)m_iterable.find_first_not_of(' '));
    return *this;
}


inline ext::string& ext::string::rtrim()
{
    // remove all the spaces from the right-hand side of the string, and return the reference to the string
    m_iterable.erase(begin() + (decltype(m_iterable)::difference_type)m_iterable.find_last_not_of(' '), end());
    return *this;
}


inline ext::string& ext::string::trim()
{
    // remove all the spaces from both sides of the string, and return the reference to the string
    ltrim();
    rtrim();
    return *this;
}


inline ext::string& ext::string::to_lowercase()
{
    // convert the string to lowercase by transforming each character individually, and return a reference to thr string
    std::transform(m_iterable.begin(), m_iterable.end(), m_iterable.begin(), [](char c){return std::tolower(c);});
    return *this;
}


inline ext::string& ext::string::to_uppercase()
{
    // convert the string to uppercase by transforming each character individually, and return a reference to thr string
    std::ranges::transform(m_iterable.begin(), m_iterable.end(), m_iterable.begin(), [](char c){return std::toupper(c);});
    return *this;
}


inline ext::string& ext::string::new_lowercase() const
{
    // create a new string, convert it to lowercase, and return it
    return string{*this}.to_lowercase();
}


inline ext::string& ext::string::new_uppercase() const
{
    // create a new string, convert it to uppercase, and return it
    return string{*this}.to_uppercase();
}


inline ext::string ext::string::substring(const size_t offset, const size_t count) const
{
    // create a substring from the internal value and wrap it into ext::string, and return it
    string substring;
    return substring = m_iterable.substr(offset, count);
}


inline ext::string ext::string::replace(size_t offset, size_t count, cstring& replacement)
{
    // replace count characters starting at offset with replacement
    m_iterable.replace(offset, count, replacement);
}


inline ext::vector<ext::string> ext::string::split(char delimiter, size_t max_delimiters) const
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


inline bool ext::string::contains(const char* item) const
{
    // check if the string contains a substring
    return m_iterable.contains(item);
}


/**
 * use the .c_str() to keep compatibility with std::string::c_str() -> also helps with messy (const char*) conversions
 * @return const char* string
 */
inline constexpr const char* ext::string::c_str() const
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


inline ext::string ext::string::operator+(cstring& other) const
{
    // create a new string composed of adding the two strings, and return it
    ext::string new_string;
    return new_string = m_iterable + other.m_iterable;
}


inline ext::string ext::string::operator+(const char* other) const
{
    // create a new string composed of adding the two strings, and return it
    ext::string new_string;
    return new_string = m_iterable + other;
}


inline ext::string& ext::string::operator+=(cstring& other)
{
    // acd the other string to this string and return a reference to it
    m_iterable += other.m_iterable;
    return *this;
}


inline bool ext::string::operator<(cstring& other) const
{
    // do comparisons by pointer comparison
    return this < &other;
}


inline bool ext::string::operator>(cstring& other) const
{
    // do comparisons by pointer comparison
    return this > &other;
}


inline bool ext::string::operator!() const
{
    // check if the string is empty
    return empty();
}


inline bool ext::string::operator==(const string& other) const
{
    // equality check by comparing the two strings
    return m_iterable == other.m_iterable;
}


inline bool ext::string::operator==(const char* other) const
{
    // inequality check by comparing the two strings
    return m_iterable.c_str() == other;
}


template <class ...string_t>
ext::string ext::concatenate_strings(string_t&&... strings) {

    // create a string stream and add the forwarded strings into it
    const std::ostringstream stream;
    (stream << ... << std::forward<string_t>(strings));

    // convert the stream into a string, and return it
    ext::string concatenation;
    return concatenation = stream.str();
}


#endif //SBROWSER_STRING_HPP
