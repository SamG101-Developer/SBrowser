#ifndef SBROWSER_TYPE_TRAITS_HPP
#define SBROWSER_TYPE_TRAITS_HPP

#include <functional>
#include <optional>
#include <type_traits>

#include <ext/smart_pointer.hpp>


namespace ext {template <template <typename> typename T, typename U> struct is_mixin_base_of;}
namespace ext{template <template <typename> typename T, typename U> inline constexpr bool is_mixin_base_of_v = is_mixin_base_of<T, U>::value;}

namespace ext {template <typename T> struct is_smart_ptr;}
namespace ext {template <typename T> inline constexpr bool is_smart_ptr_v = is_smart_ptr<T>::value;}

namespace ext {template <typename T, class = void> struct is_iterator;}
namespace ext {template <typename T> inline constexpr bool is_iterator_v = is_iterator<T>::value;}

namespace ext {template <typename T, class = void> struct is_iterable;}
namespace ext {template <typename T> constexpr bool is_iterable_v = is_iterable<T>::value;}

namespace ext {template <typename ...Args> auto is_inf(Args&&... value) -> bool;}
namespace ext {template <typename ...Args> auto is_nan(Args&&... value) -> bool;}
namespace ext {template <typename ...Args> auto is_inf_or_nan(Args&&... value) -> bool;}


template <template <typename> typename T, typename U>
struct ext::is_mixin_base_of
{
private:
    template <typename V>
    static auto test(const T<V>&) -> decltype(static_cast<const T<V>&>(std::declval<U>()), std::true_type{});
    static auto test(...) -> std::false_type;

public:
    static constexpr bool value = decltype(is_mixin_base_of::test(std::declval<U>()))::value;
};


template <typename T> struct ext::is_smart_ptr : std::false_type {};
template <typename T> struct ext::is_smart_ptr<smart_pointer<T>> : std::true_type {};

template <typename T, class> struct ext::is_iterator : std::false_type {};
template <typename T> struct ext::is_iterator<T, std::void_t<typename std::iterator_traits<T>::pointer, typename std::iterator_traits<T>::reference>> : std::true_type {};

template <typename T, class> struct ext::is_iterable : std::false_type {};
template <typename T> struct ext::is_iterable<T, std::void_t<decltype(std::begin(std::declval<T>())), decltype(std::end(std::declval<T>()))>> : std::true_type {};


template <typename ...Args>
auto ext::is_inf(Args&&... value) -> bool
{
    // check if any of the values are infinity
    return (std::isinf(std::forward<Args>(value)) || ...);
}


template <typename ...Args>
auto ext::is_nan(Args&&... value) -> bool
{
    // check if any of the values are NaNs
    return (std::isnan(std::forward<Args>(value)) || ...);
}


template <typename ...Args>
auto ext::is_inf_or_nan(Args&&... value) -> bool
{
    // check if any of the values are infinity or NaNs
    return ((is_inf(value) or is_nan(value)) || ...);
}


// unsigned counterparts to the char/short/int/long/longlong primitives (not post-fixed with _t for consistency)
using uchar     = unsigned char;
using ushort    = unsigned short;
using uint      = unsigned int;
using ulong     = unsigned long;
using ulonglong = unsigned long long;


// equality operators for types that don't have them defined
template <typename T, typename ...Args>
auto operator==(
        const std::function<T(Args...)>& function_a,
        const std::function<T(Args...)>& function_b)
        -> bool
{
    // equality check by comparing the addresses of the two functions
    return &function_a == &function_b;
}


template <typename T, typename ...Args>
auto operator<=>(
        const std::function<T(Args...)>& function_a,
        const std::function<T(Args...)>& function_b)
        -> bool
{
    // comparison check by comparing the addresses of the two functions
    return &function_a <=> &function_b;
}


#endif //SBROWSER_TYPE_TRAITS_HPP
