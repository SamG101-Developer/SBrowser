#ifndef SBROWSER_HTML_PROPERTY_HPP
#define SBROWSER_HTML_PROPERTY_HPP

#include <ext/dom_property.hpp>

#include <QtCore/QPointer>

namespace ext {template <typename T, bool ce_reactions> struct html_property;}

template <typename T, bool ce_reactions=_F>
struct ext::html_property : public dom_property<T, ce_reactions>
{
public aliases:
    using qt_updater_t = std::function<void(T)>;

public constructors:
    html_property() = default;

public cpp_methods:
    template <typename F, typename U> auto attach_qt_updater(F method, U pointer) -> void;
    auto constrain_values(const std::initializer_list<T>& constrain_to);

public operators:
    auto operator=(const T& o) -> html_property<T, ce_reactions>& override;
    auto operator=(T&& o) noexcept -> html_property<T, ce_reactions>& override;

private:
    bool m_qt_updater_attached = false;
    qt_updater_t m_qt_updater;
    ext::vector<T> m_constrain_to = {};
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
    m_qt_updater = std::bind(method, pointer->qt());
}


template <typename T, bool ce_reactions>
auto ext::html_property<T, ce_reactions>::constrain_values(const std::initializer_list<T>& constrain_to)
{
    // set the value constraints to a vector of values ie value must be in the vector
    m_constrain_to = ext::vector<T>{constrain_to};
}


template <typename T, bool ce_reactions>
auto ext::html_property<T, ce_reactions>::operator=(const T& o) -> html_property<T, ce_reactions>&
{
    // only continue if the constraints are empty or contain the new value
    if (m_constrain_to.empty() or m_constrain_to.contains(o))
    {
        // execute the updater if there is one, and perform default setting operations (for const reference)
        if (m_qt_updater_attached) m_qt_updater();
        dom_property<T, ce_reactions>::operator=(o);
    }
}


template <typename T, bool ce_reactions>
auto ext::html_property<T, ce_reactions>::operator=(T&& o) noexcept -> html_property<T, ce_reactions>&
{
    // only continue if the constraints are empty or contain the new value
    if (m_constrain_to.empty() or m_constrain_to.contains(o))
    {
        // execute the updater if there is one, and perform default setting operations (for movable)
        if (m_qt_updater_attached) m_qt_updater();
        dom_property<T, ce_reactions>::operator=(std::move(o));
    }
}


#endif //SBROWSER_HTML_PROPERTY_HPP
