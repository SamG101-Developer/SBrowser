#include "html_or_svg_element.hpp"

#include <html/elements/html_element.hpp>


template <typename T>
auto html::mixins::html_or_svg_element<T>::blur() -> void
{
    auto* base = static_cast<T*>(this);
    base->qt()->clearFocus();
}


template <typename T>
auto html::mixins::html_or_svg_element<T>::focus(
        const ext::string_any_map& options)
        -> void
{
    auto prevent_scroll = options.at("preventScroll").template to<bool>(); // TODO : something here
    auto* base = static_cast<T*>(this);
    base->qt()->setFocus();
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
