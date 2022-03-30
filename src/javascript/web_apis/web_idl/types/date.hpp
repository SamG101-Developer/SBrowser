#pragma once
#ifndef SBROWSER_DATE_HPP
#define SBROWSER_DATE_HPP

#include <ext/decorators.hpp>

namespace webidl::types {class date;}


class webidl::types::date {
public: constructors
    date(double time);

public js_methods:
    const char* year();
    const char* month();
    const char* day();
    const char* hour();
    const char* minute();
    const char* second();
    const char* milliseconds();

public: operators
    operator double() const;

private: cpp_properties
    double m_time;
};


webidl::types::date::date(double time): m_time(time)
{
}


webidl::types::date::operator double() const
{
    return m_time;
}


#endif //SBROWSER_DATE_HPP
