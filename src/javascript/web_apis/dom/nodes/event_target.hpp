#pragma once
#ifndef SBROWSER_EVENT_TARGET_HPP
#define SBROWSER_EVENT_TARGET_HPP

#include <functional>

#include <ext/iterables/map.hpp>
#include <ext/iterables/string.hpp>
#include <ext/iterables/vector.hpp>
#include <ext/macros/decorators.hpp>

#include <dom_object.hpp>

namespace dom {
    namespace events {class event;}
    namespace helpers {
        struct event_dispatching;
        struct event_listening;
    }
    namespace nodes {class event_target;}
}


class dom::nodes::event_target : public dom_object {
friends
    friend struct helpers::event_dispatching;
    friend struct helpers::event_listening;
    using event_listener_callback = std::function<void()>;

public: constructors
    event_target();

    ~event_target() override;

public: methods
    void add_event_listener(ext::string type, event_listener_callback&& callback, ext::cstring_any_map& options);
    void remove_event_listener(ext::string type, event_listener_callback&& callback, ext::cstring_any_map& options);
    bool dispatch_event(events::event* event);

protected: internal_methods
    virtual event_target* get_the_parent(events::event* event);

public: internal_methods
    ext::any&& v8(v8::Isolate *isolate) const override;

private: internal_properties
    ext::vector<ext::string_any_map> m_event_listeners;
};


#endif //SBROWSER_EVENT_TARGET_HPP
