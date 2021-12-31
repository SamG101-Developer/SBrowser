#pragma once
#ifndef SBROWSER_WINDOW_HPP
#define SBROWSER_WINDOW_HPP

#include <ext/mixins/listlike.hpp>
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
//        , public html::mixins::window_or_worker_global_scope
//        , public css::cssom_view::mixins::scrollable
        , public ext::listlike<ext::string> {

public constructors:
    window();

public methods:
    // current browsing context
    void close();
    void stop();
    void focus();

    window_proxy* open(ext::cstring& url = "", ext::cstring& target = "", ext::cstring& features="");

    // user prompts
    void print();
    void alert(ext::cstring& message = "");
    void confirm(ext::cstring& message = "");
    ext::string prompt(ext::cstring& message="", ext::cstring& default_ = "");
    void post_message(ext::string message, ext::cstring_any_map& options = {});

public methods:
    new_obj css::cssom::other::css_style_declaration* get_computed_style(element* elem, ext::cstring& pseudo_element);


public methods:
    // spatial navigation
    void navigate(ext::cstring& direction);

public methods:
    // cssom-view browsing context
    void move_to(long x, long y);
    void move_by(long x, long y);
    void resize_to(long width, long height);
    void resize_by(long width, long height);

public properties:
    // current browsing context
    ext::dom_property<ext::string> name;
    ext::dom_property<ext::string> status;
    ext::dom_property<bool> closed;

    ext::dom_property<html::other::location*> location;
    ext::dom_property<html::other::history*> history;
    ext::dom_property<html::custom_elements::custom_element_registry*> custom_elements;

    ext::dom_property<window_proxy*> window;
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
    ext::dom_property<unsigned long> length;
    ext::dom_property<element*> frame_element;
    ext::dom_property<window_proxy*> frames;
    ext::dom_property<window_proxy*> top;
    ext::dom_property<window_proxy*> parent;

    // user agent
    ext::dom_property<bool> origin_agent_cluster;
    ext::dom_property<html::other::navigator*> navigator;

public properties:
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

public properties:
    // screens
    ext::dom_property<css::cssom_view::other::screen*> screen;

private accessors:
    long get_inner_width();
    long get_inner_height();
    long get_outer_width();
    long get_outer_height();

    double get_scroll_x();
    double get_scroll_y();
    double get_page_x_offset();
    double get_page_y_offset();
};


#endif //SBROWSER_WINDOW_HPP
