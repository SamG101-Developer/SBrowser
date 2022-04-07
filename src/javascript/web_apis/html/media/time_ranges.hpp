#pragma once
#ifndef SBROWSER_TIME_RANGES_HPP
#define SBROWSER_TIME_RANGES_HPP

#include <dom_object.hpp>
#include <ext/listlike.hpp>

namespace html::media {class time_ranges;}


class html::media::time_ranges
        : virtual public dom_object
        , public ext::listlike<std::pair<int, int>>
{
public constructors:
    time_ranges();

public js_methods:
    auto start(unsigned long index) const -> double;
    auto end(unsigned long index) const -> double;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_TIME_RANGES_HPP
