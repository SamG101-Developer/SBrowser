#pragma once
#ifndef SBROWSER_ORDERED_SETS_HPP
#define SBROWSER_ORDERED_SETS_HPP

#include <ext/iterables/set.hpp>
#include <ext/iterables/string.hpp>

namespace dom::helpers {struct ordered_sets;}


struct dom::helpers::ordered_sets {
public:
    static ext::set<ext::string> ordered_set_parser(ext::cstring& input_string);
    static ext::string ordered_set_serializer(const ext::set<ext::string>& input_set);
};


#endif //SBROWSER_ORDERED_SETS_HPP
