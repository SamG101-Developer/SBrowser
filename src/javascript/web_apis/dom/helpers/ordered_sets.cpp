#include "ordered_sets.hpp"


auto dom::helpers::ordered_sets::ordered_set_parser(ext::cstring& input_string) -> ext::set<ext::string>
{
    // create an empty set, and emplace each element from the space-split string
    ext::set<ext::string> output_set;
    input_string.split(' ').for_each([&output_set](ext::cstring& string) {output_set.push(string);});

    // return the set of strings
    return output_set;
}


auto dom::helpers::ordered_sets::ordered_set_serializer(const ext::set<ext::string>& input_set) -> ext::string
{
    // TODO
}