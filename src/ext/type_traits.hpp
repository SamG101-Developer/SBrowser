#ifndef SBROWSER_TYPE_TRAITS_HPP
#define SBROWSER_TYPE_TRAITS_HPP

#include <optional>
#include <type_traits>


namespace ext
{
    template <template <typename> typename T, typename U> struct is_mixin_base_of;
    template <template <typename> typename T, typename U> inline constexpr bool is_mixin_base_of_v = is_mixin_base_of<T, U>::value;
}


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


// equality operators for types that don't ahev them defined
auto operator==(const std::function<void()>& function_a, const std::function<void()>& function_b) -> bool
{
    // equality check by comparing the addresses of the two objects
    return &function_a == &function_b;
}


#endif //SBROWSER_TYPE_TRAITS_HPP
