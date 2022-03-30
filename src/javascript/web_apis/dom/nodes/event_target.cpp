#include "event_target.hpp"

#include <dom/events/event.hpp>

#include <dom/helpers/event_listening.hpp>
#include <dom/helpers/exceptions.hpp>


dom::nodes::event_target::~event_target()
{
    // clear the event listeners
    m_event_listeners.clear();
}


auto dom::nodes::event_target::add_event_listener(
        ext::string type,
        event_listener_callback&& callback,
        const ext::string_any_map& options)
        -> void
{
    // create an event listener that is the flattened options, and insert the callback and type
    auto event_listener = helpers::event_listening::flatten_more(options);
    event_listener.insert("callback", std::move(callback));
    event_listener.insert("type", type);

    // add the event listener to the event target by calling the add_event_listener helper method
    helpers::event_listening::add_event_listener(this, event_listener);
}


auto dom::nodes::event_target::remove_event_listener(
        ext::string type,
        event_listener_callback&& callback,
        const ext::string_any_map& options)
        -> void
{
    // create a dummy event listener that is the flattened options, and insert the callback and type
    auto event_listener = helpers::event_listening::flatten_more(options);
    event_listener.insert("callback", std::move(callback));
    event_listener.insert("type", type);

    // remove the event listener from the event target by  calling the remove_event_listener helper method
    helpers::event_listening::remove_event_listener(this, event_listener);
}


auto dom::nodes::event_target::dispatch_event(
        events::event* const event)
        -> bool
{
    // if the dispatch is already set or the initialized flag isn't set, then throw an invalid state error
    helpers::exceptions::throw_v8_exception(
            "event must be initialized and not dispatched in order be dispatched",
            INVALID_STATE_ERR,
            [event] {return event->m_dispatch_flag or not event->m_initialized_flag;});

    // set the event trusted to false (manual dispatch), and dispatch the event through the tree
    event->is_trusted = false;
    return helpers::event_listening::dispatch(event, this);
}


auto dom::nodes::event_target::get_the_parent(
        events::event* const event)
        -> dom::nodes::event_target*
{
    // default behaviour for getting the parent in event traversal is that there is no parent
    return nullptr;
}


auto dom::nodes::event_target::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<event_target>{isolate}
            .ctor<>()
            .inherit<dom_object>()
            .function("addEventListener", &event_target::add_event_listener)
            .function("removeEventListener", &event_target::remove_event_listener)
            .function("dispatchEvent", &event_target::dispatch_event)
            .auto_wrap_objects();
}
