#include "page_transition_event.hpp"


html::events::page_transition_event::page_transition_event(
        const ext::string& event_type,
        const ext::string_any_map& event_init)

        : dom::events::event(event_type, event_init)
{
    // set the property values
    persisted = event_init.has_key("persisted") ? event_init.at("persisted").to<bool>() : false;
}
