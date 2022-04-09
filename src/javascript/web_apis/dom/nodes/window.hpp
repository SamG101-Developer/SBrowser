#pragma once
#ifndef SBROWSER_WINDOW_HPP
#define SBROWSER_WINDOW_HPP

#include <ext/listlike.hpp>
#include <ext/dom_property.hpp>
#include <javascript/interop/attribute_descriptors.hpp>

#include <dom/nodes/event_target.hpp>

namespace css::cssom_view::other {class screen;}
namespace dom::nodes {class document;}
namespace css::cssom::other {class css_style_declaration;}
namespace dom::nodes {class element;}
namespace dom::nodes {class window;}
namespace dom::nodes {class window_proxy;}
namespace html::custom_elements {struct custom_element_registry;}
namespace html::other {class location;}
namespace html::other {class history;}
namespace html::other {class navigator;}
namespace html::other {class bar_prop;}


class dom::nodes::window
        : public event_target
        // , public html::mixins::window_or_worker_global_scope
        // , public css::cssom_view::mixins::scrollable
        , public ext::listlike<ext::string>
{
public constructors:
    window();

public js_methods:
    // current browsing context
    auto close() -> void;
    auto stop() -> void;
    auto focus() -> void;

    auto open(const ext::string& url = "", const ext::string& target = "", const ext::string& features="") -> window_proxy*;

    // user prompts
    auto print() -> void;
    auto alert(const ext::string& message = "") const -> void;
    auto confirm(const ext::string& message = "") const -> void;
    auto prompt(const ext::string& message="", const ext::string& default_ = "") const -> ext::string;
    auto post_message(ext::string message, const ext::string_any_map& options = {}) -> void;

public: js_methods
    new_obj auto get_computed_style(element* elem, const ext::string& pseudo_element) -> css::cssom::other::css_style_declaration*;

public: js_methods
    // spatial navigation
    auto navigate(const ext::string& direction) -> void;

public: js_methods
    // cssom-view browsing context
    auto move_to(long x, long y) -> void;
    auto move_by(long x, long y) -> void;
    auto resize_to(long width, long height) -> void;
    auto resize_by(long width, long height) -> void;

public: js_properties
    // current browsing context
    ext::dom_property<ext::string> name;
    ext::dom_property<ext::string> status;
    ext::dom_property<bool> closed;

    ext::dom_property<html::other::location*> location;
    ext::dom_property<html::other::history*> history;
    ext::dom_property<html::custom_elements::custom_element_registry*> custom_elements;

    ext::dom_property<window_proxy*> window_;
    ext::dom_property<window_proxy*> self;
    ext::dom_property<document*> document;

    // bar prop elements
    ext::dom_property<html::other::bar_prop*> location_bar;
    ext::dom_property<html::other::bar_prop*> menu_bar;
    ext::dom_property<html::other::bar_prop*> personal_bar;
    ext::dom_property<html::other::bar_prop*> scroll_bars;
    ext::dom_property<html::other::bar_prop*> status_bar;
    ext::dom_property<html::other::bar_prop*> tool_bar;

    // animated types browsing context
    ext::dom_property<ext::any> opener;
    ext::dom_property<ulong> length;
    ext::dom_property<element*> frame_element;
    ext::dom_property<window_proxy*> frames;
    ext::dom_property<window_proxy*> top;
    ext::dom_property<window_proxy*> parent;

    // user agent
    ext::dom_property<bool> origin_agent_cluster;
    ext::dom_property<html::other::navigator*> navigator;

public: js_properties
    // viewport
    ext::dom_property<long> inner_width;
    ext::dom_property<long> inner_height;

    // viewport scrolling
    ext::dom_property<double> scroll_x;
    ext::dom_property<double> scroll_y;
    ext::dom_property<double> page_x_offset;
    ext::dom_property<double> page_y_offset;

    // client
    ext::dom_property<long> screen_x;
    ext::dom_property<long> screen_y;
    ext::dom_property<long> outer_width;
    ext::dom_property<long> outer_height;
    ext::dom_property<double> device_pixel_width;

public: js_properties
    // screens
    ext::dom_property<css::cssom_view::other::screen*> screen;

public: cpp_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;

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
