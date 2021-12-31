#include "counting.hpp"

#include <console/abstract.hpp>


void
console::counting::count(ext::cstring& label) {

//    count_map.at(label) = count_map.has_key(label)
//        ? count_map.at(label) += 1
//        : 1; TODO

//    ext::string concatenation = label + ": " + std::to_string(count_map.at(label)).c_str();
//    abstract::logger("count", {concatenation});
}


void
console::counting::count_reset(ext::cstring& label) {
    if (count_map.has_key(label))
        count_map.at(label) = 0;
    else
        abstract::logger("countReset", "Label doesn't have a count");

}
