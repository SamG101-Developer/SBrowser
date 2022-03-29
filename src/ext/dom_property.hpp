#ifndef SBROWSER_DOM_PROPERTY_HPP
#define SBROWSER_DOM_PROPERTY_HPP

#include <ext/property.hpp>

namespace ext {template <typename T, bool ce_reactions> struct dom_property;}

template <typename T, bool ce_reactions> void handle_ce_reactions(ext::dom_property<T, ce_reactions>& p);
template <typename function> void handle_ce_reactions(const function& f);


template <typename T, bool ce_reactions=_F>
struct ext::dom_property : public property<T>
{
public constructors:
    dom_property() = default;
    dom_property(const T& val) {this->m_internal = val;}
    dom_property(T&& val) {this->m_internal = std::forward<T&>(val);}
    dom_property(const dom_property<T, ce_reactions>&) = default;
    ~dom_property() override;

public operators:
    operator T() const override;
    auto operator=(const T& o) -> dom_property<T, ce_reactions>& override;
    auto operator=(T&& o) noexcept -> dom_property<T, ce_reactions>& override;
};


template <typename T, bool ce_reactions>
_FAST _INLINE ext::dom_property<T, ce_reactions>::~dom_property()
{
    // handle any custom element reactions, and perform default deletion operations
    if constexpr(ce_reactions) handle_ce_reactions(*this);
}


template <typename T, bool ce_reactions>
_FAST _INLINE ext::dom_property<T, ce_reactions>::operator T() const
{
    // handle any custom element reactions, and perform default getting operations
    if constexpr(ce_reactions) handle_ce_reactions(*this);
    return property<T>::operator T();
}


template <typename T, bool ce_reactions>
_FAST _INLINE auto ext::dom_property<T, ce_reactions>::operator=(
        const T& o)
        -> dom_property<T, ce_reactions>&
{
    // handle any custom element reactions, and perform default setting operations (for const reference)
    if constexpr(ce_reactions) handle_ce_reactions(*this);
    property<T>::operator=(o);
    return *this;
}


template <typename T, bool ce_reactions>
_FAST _INLINE auto ext::dom_property<T, ce_reactions>::operator=(
        T&& o) noexcept
        -> dom_property<T, ce_reactions>&
{
    // handle any custom element reactions, and perform default setting operations (for movable)
    if constexpr(ce_reactions) handle_ce_reactions(*this);
    property<T>::operator=(std::move(o));
    return *this;
}


#endif //SBROWSER_DOM_PROPERTY_HPP
