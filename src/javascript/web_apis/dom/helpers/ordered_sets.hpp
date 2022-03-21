#pragma once
#ifndef SBROWSER_ORDERED_SETS_HPP
#define SBROWSER_ORDERED_SETS_HPP

#include <ext/set.hpp>
#include <ext/string.hpp>

namespace dom::helpers {struct ordered_sets;}


/*
 * Group of helper methods designed to help with parsing and serializing data to and from ordered sets
 */
struct dom::helpers::ordered_sets
{
    static auto ordered_set_parser(const ext::string& input_string) -> ext::set<ext::string>;
    static auto ordered_set_serializer(const ext::set<ext::string>& input_set) -> ext::string;
};


#endif //SBROWSER_ORDERED_SETS_HPP
