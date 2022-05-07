#ifndef SBROWSER_TYPE_TRAITS_HPP
#define SBROWSER_TYPE_TRAITS_HPP

#include <optional>
#include <type_traits>


namespace ext
{
    template <template <typename> typename T, typename U> struct is_mixin_base_of;
    template <template <typename> typename T, typename U> inline constexpr bool is_mixin_base_of_v = is_mixin_base_of<T, U>::value;
}

namespace ext
{
    template <typename T, typename ...Args> struct is_invokable;
    template <typename T, typename ...Args> inline constexpr bool is_invokable_v = is_invokable<T, Args...>::value;
}


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


template <typename T, typename ...Args>
struct ext::is_invokable
{
private:
    template <typename U>
    static auto test(U* p) -> decltype((*p)(std::declval<Args>()...), void(), std::true_type{});
    static auto test(...) -> std::false_type;

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

// unsigned counterparts to the char/short/int/long/longlong primitives (not post-fixed with _t for consistency)
using uchar     = unsigned char;
using ushort    = unsigned short;
using uint      = unsigned int;
using ulong     = unsigned long;
using ulonglong = unsigned long long;

// syntactic sugar around std::nullopt
inline constexpr std::nullopt_t null{std::nullopt_t::_Tag{}};


#endif //SBROWSER_TYPE_TRAITS_HPP
