#pragma once
#ifndef SBROWSER_EVENT_TARGET_HPP
#define SBROWSER_EVENT_TARGET_HPP

#include <functional>
#include <ext/macros/decorators.hpp>
#include <ext/iterables/map.hpp>
#include <ext/iterables/string.hpp>
#include <ext/iterables/vector.hpp>

namespace dom {
    namespace events {class event;}
    namespace helpers {
        struct event_dispatching;
        struct event_listening;
    }
    namespace nodes {class event_target;}
}


class dom::nodes::event_target {
friends
    friend struct helpers::event_dispatching;
    friend struct helpers::event_listening;
    using event_listener_callback = std::function<void()>;

public constructors:
    event_target() = default;
    event_target(const event_target&) = default;
    event_target(event_target&&) noexcept = default;
    event_target& operator=(const event_target&) = default;
    event_target& operator=(event_target&&) noexcept = default;

    virtual ~event_target() {m_event_listeners.clear(true);};

public methods:
    void add_event_listener(ext::string type, event_listener_callback&& callback, ext::cstring_any_map& options);
    void remove_event_listener(ext::string type, event_listener_callback&& callback, ext::cstring_any_map& options);
    bool dispatch_event(events::event* event);

protected internal_methods:
    virtual event_target* get_the_parent(events::event* event);

private internal_properties:
    ext::vector<ext::string_any_map> m_event_listeners;
};


#endif //SBROWSER_EVENT_TARGET_HPP
