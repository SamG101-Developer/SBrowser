#pragma once
#ifndef SBROWSER_EVENT_TARGET_HPP
#define SBROWSER_EVENT_TARGET_HPP

#include <functional>
#include <ext/any.hpp>
#include <ext/decorators.hpp>
#include <ext/string.hpp>
#include <ext/vector.hpp>

namespace dom {
    namespace events {class event;}
    namespace helpers {
        struct event_dispatching;
        struct event_listening;
    }
    namespace nodes {class event_target;}
}


class dom::nodes::event_target {
    friend struct helpers::event_dispatching;
    friend struct helpers::event_listening;
    using event_listener_callback = std::function<void()>;

public:
    event_target()                        = default;
    event_target(const event_target&)     = default;
    event_target(event_target&&) noexcept = default;
    event_target& operator=(const event_target&)     = default;
    event_target& operator=(event_target&&) noexcept = default;
    virtual ~event_target() {m_event_listeners.clear(true);};

    void add_event_listener(ext::string type, event_listener_callback&& callback, ext::cstring_any_map& options);
    void remove_event_listener(ext::string type, event_listener_callback&& callback, ext::cstring_any_map& options);
    bool dispatch_event(events::event* event);

protected:
    virtual event_target* get_the_parent(events::event* event);

private:
    ext::vector<ext::string_any_map> m_event_listeners;
};


#endif //SBROWSER_EVENT_TARGET_HPP
