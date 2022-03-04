#pragma once
#ifndef SBROWSER_ORDERED_SETS_HPP
#define SBROWSER_ORDERED_SETS_HPP

#include <ext/iterables/set.hpp>
#include <ext/iterables/string.hpp>

namespace dom::helpers {struct ordered_sets;}


struct dom::helpers::ordered_sets
{
    static auto ordered_set_parser(const ext::string& input_string) -> ext::set<ext::string>;
    static auto ordered_set_serializer(const ext::set<ext::string>& input_set) -> ext::string;
};


#endif //SBROWSER_ORDERED_SETS_HPP
