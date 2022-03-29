#include "html_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_element::html_element()
        : dom::nodes::element{}
{
    // set the custom accessors
    inner_text.getter = [this] {return get_inner_text();};
    outer_text.getter = [this] {return get_outer_text();};

    inner_text.setter = [this](auto&& PH1) {set_inner_text(std::forward<decltype(PH1)>(PH1));};
    outer_text.setter = [this](auto&& PH1) {set_outer_text(std::forward<decltype(PH1)>(PH1));};

    offset_top.getter    = [this] {return get_offset_top();};
    offset_left.getter   = [this] {return get_offset_left();};
    offset_width.getter  = [this] {return get_offset_width();};
    offset_height.getter = [this] {return get_offset_height();};

    offset_top.setter    = [this](auto&& PH1) { set_offset_top(std::forward<decltype(PH1)>(PH1));};
    offset_left.setter   = [this](auto&& PH1) { set_offset_left(std::forward<decltype(PH1)>(PH1));};
    offset_width.setter  = [this](auto&& PH1) { set_offset_width(std::forward<decltype(PH1)>(PH1));};
    offset_height.setter = [this](auto&& PH1) { set_offset_height(std::forward<decltype(PH1)>(PH1));};

    HTML_CONSTRUCTOR
}


auto html::elements::html_element::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<html_element>{isolate}
            .template ctor<>()
            .template inherit<dom::nodes::element>()
            .template var("title", html_element::title)
            .template var("lang", html_element::lang)
            .template var("translate", html_element::translate)
            .template var("dir", html_element::dir)
            .template var("access_key_label", html_element::access_key_label)
            .template var("access_key", html_element::access_key)
            .template var("autocapitalize", html_element::autocapitalize)
            .template var("inner_text", html_element::inner_text)
            .template var("outer_text", html_element::outer_text)
            .template var("hidden", html_element::hidden)
            .template var("draggable", html_element::draggable)
            .template var("spellcheck", html_element::spellcheck)
            .template var("offset_parent", html_element::offset_parent)
            .template var("offset_top", html_element::offset_top)
            .template var("offset_left", html_element::offset_left)
            .template var("offset_width", html_element::offset_width)
            .template var("offset_height", html_element::offset_height)
            .auto_wrap_objects();
}
