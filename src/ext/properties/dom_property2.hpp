#ifndef SBROWSER_DOM_PROPERTY2_HPP
#define SBROWSER_DOM_PROPERTY2_HPP

#include <ext/properties/property2.hpp>

namespace ext {template <typename T, bool ce_reactions> struct dom_property2;}
namespace {
    template <typename T, bool ce_reactions> void handle_ce_reactions(ext::dom_property2<T, ce_reactions>& p);
}


template <typename T, bool ce_reactions>
struct ext::dom_property2 : public property2<T> {
public: constructors
    dom_property2() = default;
    dom_property2(const dom_property2<T, ce_reactions>&) = default;
    dom_property2(dom_property2<T, ce_reactions>&&) noexcept = default;
    ~dom_property2() override;

public: operators
    operator T() const override;
    dom_property2<T, ce_reactions>& operator=(const T& o) override;
    dom_property2<T, ce_reactions>& operator=(T&& o) override;
};


template <typename T, bool ce_reactions>
FAST INLINE ext::dom_property2<T, ce_reactions>::~dom_property2()
{
    // handle any custom element reactions, and perform default deletion operations
    if constexpr(ce_reactions) handle_ce_reactions(*this);
}


template <typename T, bool ce_reactions>
FAST INLINE ext::dom_property2<T, ce_reactions>::operator T() const
{
    // handle any custom element reactions, and perform default getting operations
    if constexpr(ce_reactions) handle_ce_reactions(*this);
    return property2<T>::operator T();
}


template <typename T, bool ce_reactions>
FAST INLINE ext::dom_property2<T, ce_reactions>& ext::dom_property2<T, ce_reactions>::operator=(const T& o)
{
    // handle any custom element reactions, and perform default setting operations (for const reference)
    if constexpr(ce_reactions) handle_ce_reactions(*this);
    this->set(o);
    return *this;
}


template <typename T, bool ce_reactions>
FAST INLINE ext::dom_property2<T, ce_reactions>& ext::dom_property2<T, ce_reactions>::operator=(T&& o)
{
    // handle any custom element reactions, and perform default setting operations (for const reference)
    if constexpr(ce_reactions) handle_ce_reactions(*this);
    this->set(o);
    return *this;
}


#endif //SBROWSER_DOM_PROPERTY2_HPP
