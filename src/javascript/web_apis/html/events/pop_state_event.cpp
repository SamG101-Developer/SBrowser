#include "pop_state_event.hpp"


html::events::pop_state_event::pop_state_event(
        const ext::string& event_type,
        const ext::string_any_map_t& event_init)

        : dom::events::event(event_type, event_init)
{
    // set the property's values
    state = event_init.has_key("state") ? event_init.at("state") : "";
}


auto html::events::pop_state_event::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<pop_state_event>{isolate}
            .ctor<const ext::string&, const ext::string_any_map_t&>()
            .inherit<dom::events::event>()
            .var("state", &pop_state_event::state)
            .auto_wrap_objects();
}
