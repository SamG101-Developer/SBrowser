#include "ordered_sets.hpp"


auto dom::helpers::ordered_sets::ordered_set_parser(const ext::string& input_string) -> ext::set<ext::string>
{
    // create an empty set, and emplace each element from the space-split string
    ext::set<ext::string> output_set;
    for (auto string: input_string.split(' '))
        output_set.add(string);

    // return the set of strings
    return output_set;
}


auto dom::helpers::ordered_sets::ordered_set_serializer(const ext::set<ext::string>& input_set) -> ext::string
{
    // create an empty string, and append each element from the set
    ext::string output_string;
    for (auto element: input_set)
        output_string += element;

    // return the string
    return output_string;
}
