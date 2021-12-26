#include "ordered_sets.hpp"


ext::set<ext::string>
dom::helpers::ordered_sets::ordered_set_parser(ext::cstring& input_string) {
    ext::set<ext::string> output_set;
    input_string.split(" ").for_each([&output_set](ext::cstring& string) {output_set.emplace(string);});
    return output_set;
}


ext::string
dom::helpers::ordered_sets::ordered_set_serializer(const ext::set<ext::string>& input_set) {
    ext::string output_string;
    for (ext::cstring& string: input_set) output_string += string;
    return output_string;
}
