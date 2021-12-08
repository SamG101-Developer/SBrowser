#ifndef SBROWSER_SET_HPP
#define SBROWSER_SET_HPP

#include <set>
#include <ext/iterable.hpp>

namespace ext {template <typename T> class set;}


template <typename T>
class ext::set : public ext::iterable<T, std::set<T>> {
    set() = default;
};


#endif //SBROWSER_SET_HPP
