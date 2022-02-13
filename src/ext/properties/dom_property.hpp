#ifndef SBROWSER_DOM_PROPERTY_HPP
#define SBROWSER_DOM_PROPERTY_HPP

#include <ext/properties/property.hpp>

namespace ext {template <typename T, bool ce_reactions> struct dom_property;}

template <typename T, bool ce_reactions> void handle_ce_reactions(ext::dom_property<T, ce_reactions>& p);
template <typename function> void handle_ce_reactions(const function& f);


template <typename T, bool ce_reactions>
struct ext::dom_property : public property<T> {
public: constructors
    dom_property() = default;
    dom_property(const dom_property<T, ce_reactions>&) = default;
    ~dom_property() override;

public: operators
    operator T() const override;
    dom_property<T, ce_reactions>& operator=(const T& o) override;
    dom_property<T, ce_reactions>& operator=(T&& o) override;
};


template <typename T, bool ce_reactions>
FAST INLINE ext::dom_property<T, ce_reactions>::~dom_property()
{
    // handle any custom element reactions, and perform default deletion operations
    if constexpr(ce_reactions) handle_ce_reactions(*this);
}


template <typename T, bool ce_reactions>
FAST INLINE ext::dom_property<T, ce_reactions>::operator T() const
{
    // handle any custom element reactions, and perform default getting operations
    if constexpr(ce_reactions) handle_ce_reactions(*this);
    return property<T>::operator T();
}


template <typename T, bool ce_reactions>
FAST INLINE ext::dom_property<T, ce_reactions>& ext::dom_property<T, ce_reactions>::operator=(const T& o)
{
    // handle any custom element reactions, and perform default setting operations (for const reference)
    if constexpr(ce_reactions) handle_ce_reactions(*this);
    this->set(o);
    return *this;
}


template <typename T, bool ce_reactions>
FAST INLINE ext::dom_property<T, ce_reactions>& ext::dom_property<T, ce_reactions>::operator=(T&& o)
{
    // handle any custom element reactions, and perform default setting operations (for const reference)
    if constexpr(ce_reactions) handle_ce_reactions(*this);
    this->set(o);
    return *this;
}


#endif //SBROWSER_DOM_PROPERTY_HPP
