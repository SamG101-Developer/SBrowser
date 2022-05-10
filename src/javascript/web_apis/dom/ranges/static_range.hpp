#pragma once
#ifndef SBROWSER_STATIC_RANGE_HPP
#define SBROWSER_STATIC_RANGE_HPP

#include <ext/map.hpp>
#include <dom/ranges/abstract_range.hpp>

namespace dom::ranges {class static_range;}


class dom::ranges::static_range : public abstract_range
{
public constructors:
    static_range();
    explicit static_range(const ext::string_any_map_t& init);

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_STATIC_RANGE_HPP
