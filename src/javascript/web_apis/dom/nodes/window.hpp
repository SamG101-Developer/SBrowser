#pragma once
#ifndef SBROWSER_WINDOW_HPP
#define SBROWSER_WINDOW_HPP

#include <ext/mixins/listlike.hpp>
#include <ext/properties/dom_property.hpp>
#include <javascript/interop/attribute_descriptors.hpp>

#include <dom/nodes/event_target.hpp>

namespace dom::nodes {
    class document;
    class element;
    class window;
    class window_proxy;
}

namespace html {
    namespace custom_elements {struct custom_element_registry;}

    namespace other {
        class location;
        class history;
        class navigator;
        class bar_prop;
    }
}

namespace css {
    namespace cssom_view::other {class screen;}
    namespace cssom::other {class css_style_declaration;}
}


class dom::nodes::window
        : public event_target
        // , public html::mixins::window_or_worker_global_scope
        // , public css::cssom_view::mixins::scrollable
        , public ext::listlike<ext::string> {

public constructors:
    window();

public methods:
    // current browsing context
    auto close() -> void;
    auto stop() -> void;
    auto focus() -> void;

    auto open(ext::cstring& url = "", ext::cstring& target = "", ext::cstring& features="") -> window_proxy*;

    // user prompts
    auto print() -> void;
    auto alert(ext::cstring& message = "") -> void;
    auto confirm(ext::cstring& message = "") -> void;
    auto prompt(ext::cstring& message="", ext::cstring& default_ = "") -> ext::string;
    auto post_message(ext::string message, ext::cstring_any_map& options = {}) -> void;

public: methods
    new_obj auto get_computed_style(element* elem, ext::cstring& pseudo_element) -> css::cssom::other::css_style_declaration*;

public: methods
    // spatial navigation
    auto navigate(ext::cstring& direction) -> void;

public: methods
    // cssom-view browsing context
    auto move_to(long x, long y) -> void;
    auto move_by(long x, long y) -> void;
    auto resize_to(long width, long height) -> void;
    auto resize_by(long width, long height) -> void;

public: properties
    // current browsing context
    ext::dom_property<ext::string, _F> name;
    ext::dom_property<ext::string, _F> status;
    ext::dom_property<bool, _F> closed;

    ext::dom_property<html::other::location*, _F> location;
    ext::dom_property<html::other::history*, _F> history;
    ext::dom_property<html::custom_elements::custom_element_registry*, _F> custom_elements;

    ext::dom_property<window_proxy*, _F> window_;
    ext::dom_property<window_proxy*, _F> self;
    ext::dom_property<document*, _F> document;

    // bar prop elements
    ext::dom_property<html::other::bar_prop*, _F> location_bar;
    ext::dom_property<html::other::bar_prop*, _F> menu_bar;
    ext::dom_property<html::other::bar_prop*, _F> personal_bar;
    ext::dom_property<html::other::bar_prop*, _F> scroll_bars;
    ext::dom_property<html::other::bar_prop*, _F> status_bar;
    ext::dom_property<html::other::bar_prop*, _F> tool_bar;

    // animated types browsing context
    ext::dom_property<ext::any, _F> opener;
    ext::dom_property<unsigned long, _F> length;
    ext::dom_property<element*, _F> frame_element;
    ext::dom_property<window_proxy*, _F> frames;
    ext::dom_property<window_proxy*, _F> top;
    ext::dom_property<window_proxy*, _F> parent;

    // user agent
    ext::dom_property<bool, _F> origin_agent_cluster;
    ext::dom_property<html::other::navigator*, _F> navigator;

public: properties
    // viewport
    ext::dom_property<long, _F> inner_width;
    ext::dom_property<long, _F> inner_height;

    // viewport scrolling
    ext::dom_property<double, _F> scroll_x;
    ext::dom_property<double, _F> scroll_y;
    ext::dom_property<double, _F> page_x_offset;
    ext::dom_property<double, _F> page_y_offset;

    // client
    ext::dom_property<long, _F> screen_x;
    ext::dom_property<long, _F> screen_y;
    ext::dom_property<long, _F> outer_width;
    ext::dom_property<long, _F> outer_height;
    ext::dom_property<double, _F> device_pixel_width;

public: properties
    // screens
    ext::dom_property<css::cssom_view::other::screen*, _F> screen;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private: accessors
    auto get_inner_width() -> long;
    auto get_inner_height() -> long;
    auto get_outer_width() -> long;
    auto get_outer_height() -> long;

    auto get_scroll_x() -> double;
    auto get_scroll_y() -> double;
    auto get_page_x_offset() -> double;
    auto get_page_y_offset() -> double;
};


#endif //SBROWSER_WINDOW_HPP
