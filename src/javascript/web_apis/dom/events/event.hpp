#pragma once
#ifndef SBROWSER_EVENT_HPP
#define SBROWSER_EVENT_HPP

#include <ext/iterables/map.hpp>
#include <ext/iterables/string.hpp>
#include <ext/macros/decorators.hpp>
#include <ext/properties/dom_property.hpp>

#include <dom_object.hpp>

namespace dom
{
    namespace events {class event;}
    namespace helpers
    {
        struct event_dispatching;
        struct event_listening;
    }
    namespace internal {struct event_path_struct;}
    namespace nodes {class event_target;}
}


class dom::events::event : virtual public dom_object
{
public: friends
    friend class dom::nodes::event_target;
    friend struct dom::helpers::event_dispatching;
    friend struct dom::helpers::event_listening;

public: constructors
    event();
    event(ext::cstring& event_type, ext::cstring_any_map& event_init = {});

    ~event() override;

public: static_constants
    static constexpr unsigned char NONE = 0;
    static constexpr unsigned char CAPTURING_PHASE = 1;
    static constexpr unsigned char AT_TARGET = 2;
    static constexpr unsigned char BUBBLING_PHASE = 3;

public: methods
    auto stop_propagation() -> void;
    auto stop_immediate_propagation() -> void;
    auto prevent_default() -> void;
    auto composed_path() const -> ext::vector<nodes::event_target*>;

public: properties
    ext::dom_property<ext::string, _F> type;
    ext::dom_property<bool, _F> bubbles;
    ext::dom_property<bool, _F> cancelable;
    ext::dom_property<bool, _F> composed;
    ext::dom_property<nodes::event_target*, _F> target;
    ext::dom_property<nodes::event_target*, _F> current_target;
    ext::dom_property<nodes::event_target*, _F> related_target;
    ext::dom_property<unsigned short, _F> event_phase;
    ext::dom_property<double, _F> time_stamp;
    ext::dom_property<bool, _F> is_trusted;
    ext::dom_property<ext::vector<nodes::event_target*>*, _F> touch_targets;
    ext::dom_property<ext::vector<internal::event_path_struct*>*, _F> path;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;

protected: internal_properties
    bool m_stop_propagation_flag;
    bool m_stop_immediate_propagation_flag;
    bool m_canceled_flag;
    bool m_in_passive_listener_flag;
    bool m_initialized_flag;
    bool m_dispatch_flag;
};


#endif //SBROWSER_EVENT_HPP
