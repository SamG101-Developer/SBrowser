#ifndef SBROWSER_HTML_PROPERTY_HPP
#define SBROWSER_HTML_PROPERTY_HPP

#include <ext/dom_property.hpp>

#include <QtCore/QPointer>

namespace ext {template <typename T, bool ce_reactions> struct html_property;}

template <typename T, bool ce_reactions=_F>
struct ext::html_property : public dom_property<T, ce_reactions>
{
public constructors:
    html_property() = default;

public js_methods:
    template <typename F, typename U> auto attach_qt_updater(F method, U pointer) -> void;

public operators:
    auto operator=(const T& o) -> html_property<T, ce_reactions>& override;
    auto operator=(T&& o) noexcept -> html_property<T, ce_reactions>& override;

private:
    bool m_qt_updater_attached = false;
    auto m_qt_updater;
};


template <typename T, bool ce_reactions>
template <typename F, typename U>
auto ext::html_property<T, ce_reactions>::attach_qt_updater(
        F method,
        U pointer)
        -> void
{
    // set the attached attribute to true and attach the update
    m_qt_updater_attached = true;
    m_qt_updater = std::bind(method, pointer->render());
}


template <typename T, bool ce_reactions>
auto ext::html_property<T, ce_reactions>::operator=(const T& o) -> html_property<T, ce_reactions>&
{
    // execute the updater if there is one, and perform default setting operations (for const reference)
    if (m_qt_updater_attached) m_qt_updater();
    dom_property<T, ce_reactions>::operator=(o);
}


template <typename T, bool ce_reactions>
auto ext::html_property<T, ce_reactions>::operator=(T&& o) noexcept -> html_property<T, ce_reactions>&
{
    // execute the updater if there is one, and perform default setting operations (for movable)
    if (m_qt_updater_attached) m_qt_updater();
    dom_property<T, ce_reactions>::operator=(std::move(o));
}


#endif //SBROWSER_HTML_PROPERTY_HPP
