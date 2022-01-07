#include "event_target.hpp"

#include <dom/events/event.hpp>

#include <dom/helpers/event_listening.hpp>
#include <dom/helpers/exceptions.hpp>


dom::nodes::event_target::event_target()
        : dom_object() {
}


dom::nodes::event_target::~event_target() noexcept {
    m_event_listeners.clear();
}


void
dom::nodes::event_target::add_event_listener(
        ext::string type,
        event_listener_callback&& callback,
        ext::cstring_any_map& options) {

    auto event_listener = helpers::event_listening::flatten_more(options);
    event_listener.insert("callback", callback);
    event_listener.insert("type", type);

    helpers::event_listening::add_event_listener(this, event_listener);
}


void
dom::nodes::event_target::remove_event_listener(
        ext::string type,
        event_listener_callback&& callback,
        ext::cstring_any_map& options) {

    auto event_listener = helpers::event_listening::flatten_more(options);
    event_listener.insert("callback", callback);
    event_listener.insert("type", type);

    helpers::event_listening::remove_event_listener(this, event_listener);
}


bool
dom::nodes::event_target::dispatch_event(
        events::event* event) {

    helpers::exceptions::throw_v8_exception(
            "event must be initialized and not dispatched in order be dispatched",
            INVALID_STATE_ERR,
            [event] {return event->m_dispatch_flag or not event->m_initialized_flag;});

    event->is_trusted = false;
    return helpers::event_listening::dispatch(event, this);
}


dom::nodes::event_target*
dom::nodes::event_target::get_the_parent(
        events::event* event) {

    return nullptr;
}


ext::any&& dom::nodes::event_target::v8(v8::Isolate* isolate) const {
    v8pp::class_<event_target> v8{isolate};
    return v8
            .ctor<>()
            .function("addEventListener", &dom::nodes::event_target::add_event_listener)
            .function("removeEventListener", &dom::nodes::event_target::remove_event_listener)
            .function("dispatchEvent", &dom::nodes::event_target::dispatch_event)
            .auto_wrap_objects();
}
