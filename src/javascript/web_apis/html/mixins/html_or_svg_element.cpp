#include "html_or_svg_element.hpp"

#include <html/elements/html_element.hpp>


template <typename T>
auto html::mixins::html_or_svg_element<T>::blur() -> void
{
    auto* base = static_cast<T*>(this);
    base->render()->clearFocus();
}


template <typename T>
auto html::mixins::html_or_svg_element<T>::focus(
        const ext::string_any_map& options)
        -> void
{
    auto prevent_scroll = options.at("preventScroll").template to<bool>(); // TODO : something here
    auto* base = static_cast<T*>(this);
    base->render()->setFocus();
}


template class html::mixins::html_or_svg_element<html::elements::html_element>;
// template class html::mixins::html_or_svg_element<svg::elements::svg_element>
