#pragma once
#ifndef SBROWSER_STATIC_RANGE_HPP
#define SBROWSER_STATIC_RANGE_HPP

#include <ext/iterables/map.hpp>
#include <dom/ranges/abstract_range.hpp>

namespace dom::ranges {class static_range;}


class dom::ranges::static_range : public abstract_range {
public: constructors
    static_range(): abstract_range() {};
    static_range(ext::cstring_any_map& init);

public: internal_methods
    ext::any v8(v8::Isolate *isolate) const override;
};


#endif //SBROWSER_STATIC_RANGE_HPP
