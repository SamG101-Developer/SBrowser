#ifndef SBROWSER_SMART_POINTER_HPP
#define SBROWSER_SMART_POINTER_HPP

#include <ext/shared_pointer.hpp>

#define smart_pointer ext::shared_pointer
#define make_smart ext::make_shared

namespace ext {template <typename U, typename T> auto dynamic_pointer_cast(const smart_pointer<T>& that) -> smart_pointer<U>;}

template <typename U, typename T>
auto ext::dynamic_pointer_cast(
        const smart_pointer<T>& that)
        -> smart_pointer<U>
{
    if constexpr(std::is_same_v<T, U>)
        return that;

    const auto* pointer = dynamic_cast<U*>(that.get());
    return pointer ? smart_pointer<U>(pointer) : smart_pointer<U>();
}


#endif //SBROWSER_SMART_POINTER_HPP
