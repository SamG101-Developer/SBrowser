#ifndef SBROWSER_TYPE_TRAITS_HPP
#define SBROWSER_TYPE_TRAITS_HPP

#include <memory>


template <typename T, typename U>
std::shared_ptr<T> make_unique(std::initializer_list<U> initializer_list)
{
    return std::make_unique<T>(initializer_list);
}


#endif //SBROWSER_TYPE_TRAITS_HPP
