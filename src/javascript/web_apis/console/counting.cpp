#include "counting.hpp"


void
console::counting::count(ext::cstring& label) {

    count_map.at(label) = count_map.has_key(label)
            ? count_map.at(label) + 1
            : 1;

    ext::string concatenation = label + ": " + std::to_string(count_map.at(label));
}
