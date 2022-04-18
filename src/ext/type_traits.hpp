#ifndef SBROWSER_TYPE_TRAITS_HPP
#define SBROWSER_TYPE_TRAITS_HPP

#include <type_traits>

namespace ext {template <template <typename> typename T, typename U> struct is_mixin_base_of;}
namespace ext {template <template <typename> typename T, typename U> inline constexpr bool is_mixin_base_of_v = is_mixin_base_of<T, U>::value;}

using uchar = unsigned char;
using ushort = unsigned short;
using ulong = unsigned long;
using ulonglong = unsigned long long;


template <template<typename> typename T, typename U>
struct ext::is_mixin_base_of
{
private:
    template <typename V>
    static auto test(const T<V>&) -> decltype(static_cast<const T<V>&>(std::declval<U>()), std::true_type{});
    static auto test(...) -> std::false_type;

public:
    static constexpr bool value = decltype(is_mixin_base_of::test(std::declval<U>()))::value;
};


#endif //SBROWSER_TYPE_TRAITS_HPP
