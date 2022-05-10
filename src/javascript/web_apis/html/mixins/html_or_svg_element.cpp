#include "html_or_svg_element.hpp"

#include <html/elements/html_element.hpp>
#include <html/helpers/focus_internals.hpp>


template <typename T>
auto html::mixins::html_or_svg_element<T>::blur()
        -> void
{
    html::helpers::focus_internals::unfocusing_steps(static_cast<T*>(this));

    static_cast<T*>(this)->qt()->clearFocus();
}


template <typename T>
auto html::mixins::html_or_svg_element<T>::focus(
        const ext::string_any_map_t& options)
        -> void
{
    if (m_locked_for_focus) return;

    m_locked_for_focus = true;

    html::helpers::focus_internals::focusing_steps(static_cast<T*>(this));
    auto prevent_scroll = options.at("preventScroll").template to<bool>(); // TODO : something here
    static_cast<T*>(this)->qt()->setFocus();

    m_locked_for_focus = false;
}


template <typename T>
auto html::mixins::html_or_svg_element<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_or_svg_element<T>>{isolate}
            .template inherit<dom_object>()
            .template function("blur", &html_or_svg_element<T>::blur)
            .template function("focus", &html_or_svg_element<T>::focus)
            .template var("data_set", &html_or_svg_element<T>::data_set)
            .template var("nonce", &html_or_svg_element<T>::nonce)
            .template var("autofocus", &html_or_svg_element<T>::autofocus)
            .template var("tab_index", &html_or_svg_element<T>::tab_index)
            .auto_wrap_objects();
}


template class html::mixins::html_or_svg_element<html::elements::html_element>;
// template class html::mixins::html_or_svg_element<svg::elements::svg_element>
