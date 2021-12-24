#pragma once
#ifndef SBROWSER_STATIC_RANGE_HPP
#define SBROWSER_STATIC_RANGE_HPP

#include <ext/map.hpp>
#include <dom/ranges/abstract_range.hpp>

namespace dom::ranges {class static_range;}


class dom::ranges::static_range : public abstract_range {
public: constructors
    static_range(ext::cstring_any_map& init);
};


#endif //SBROWSER_STATIC_RANGE_HPP
