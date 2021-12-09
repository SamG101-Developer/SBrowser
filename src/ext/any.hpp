#ifndef SBROWSER_ANY_HPP
#define SBROWSER_ANY_HPP

#include <any>
#include <ext/string.hpp>


//inline bool operator< (const std::any& first, const std::any& second) {return &first <  &second;}
//inline bool operator> (const std::any& first, const std::any& second) {return &first >  &second;}
//inline bool operator==(const std::any& first, const std::any& second) {return &first == &second;}


namespace ext {
    template <class T> T any_cast(std::any value);
}


template <class T>
T ext::any_cast(std::any value) {
    if (not value.has_value() or value.type() == typeid(value))
        return T(); // std::is_pointer_v<T> ? nullptr : T(); -> if constexpr () {}
    try {
        return std::any_cast<T>(value);
    } catch(...) {
        return std::any_cast<T>(std::any());
    }
}


#endif //SBROWSER_ANY_HPP
