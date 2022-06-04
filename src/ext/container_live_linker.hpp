#ifndef SBROWSER_CONTAINER_LIVE_LINKER_HPP
#define SBROWSER_CONTAINER_LIVE_LINKER_HPP

namespace ext {template <typename T, typename C> struct container_live_filter_linker   ;}
namespace ext {template <typename T, template <typename> typename C> struct container_live_cast_linker;}


#include <functional>
#include <vector>
#include <set>
#include <ext/decorators.hpp>


//// TODO -> do the if (filter function(...)) then call ->method()
//#define LIVE_FILTER_MUTABLE(method, args...)       \
//    for (auto* live_filter: this->m_live_filters)  \
//        dynamic_cast<decltype(this)>(live_filter->filtered_vector.get())->method(args)


template <typename T, typename C>
struct ext::container_live_filter_linker
{
public aliases:
    using filter_function_t = std::function<bool(T)>;

public cpp_methods:
    auto attach_live_filter(filter_function_t&& new_filter_function) -> const smart_pointer<const C>;

protected nested_class:
    struct live_filter
    {
        smart_pointer<C> filtered_vector;
        filter_function_t filter_function;
    };

protected cpp_properties:
    std::set<live_filter*> m_live_filters;
};


template <typename T, template <typename> typename C>
struct ext::container_live_cast_linker
{
public cpp_methods:
    template <typename U> auto attach_live_cast() -> const smart_pointer<const C<U>>;

public nested_class:
    template <typename U>
    struct live_transform
    {
        smart_pointer<C<U>> cast_vector;
    };

protected cpp_properties:
    std::set<live_transform<ext::any>*> m_live_transforms;
};


template <typename T, typename C>
auto ext::container_live_filter_linker<T, C>::attach_live_filter(
        filter_function_t&& new_filter_function)
        -> const smart_pointer<const C>
{
    auto new_live_filter = live_filter
    {
        .filter_function=new_filter_function,
        .filtered_vector=make_smart<C>()
    };

    m_live_filters.template emplace(make_smart<live_filter>(&new_live_filter));
    return new_live_filter;
}


template <typename T, template <typename> typename C>
template <typename U>
auto ext::container_live_cast_linker<T, C>::attach_live_cast()
        -> const ext::shared_pointer<const C<U>>
{
    auto new_live_transform = live_transform
    {
        .transformed_vector=make_smart<C<U>>()
    };

    m_live_transforms.template emplace(make_smart<live_transform>(&new_live_transform));
    return new_live_transform;
}


#define LIVE_FILTER_MUTATE_EVENT(function, arguments)                                                                                    \
    for (auto* live_filter: this->m_live_filters)                                                                                        \
        [function, arguments..., &live_filter] {std::mem_fn(function)(live_filter->filtered_vector, std::forward<args>(arguments)...);}; \


#define LIVE_FILTER_MUTATE_ADD_EVENT(function, arguments)                                                     \
    for (auto* live_filter: this->m_live_filters)                                                             \
    {                                                                                                         \
        {(                                                                                                    \
            if (live_filter->filter_function(argument))                                                       \
                [&live_filter](auto&& a) {std::mem_fn(&function)(live_filter->filtered_vector, a);}(argument) \
        , ...);}                                                                                              \
    }                                                                                                         \



#endif //SBROWSER_CONTAINER_LIVE_LINKER_HPP
