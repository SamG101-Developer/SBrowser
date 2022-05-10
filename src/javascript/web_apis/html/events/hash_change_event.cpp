#include "hash_change_event.hpp"


html::events::hash_change_event::hash_change_event(
        const ext::string& event_type,
        const ext::string_any_map_t& event_init)

        : dom::events::event(event_type, event_init)
{
    // set the property values
    old_url = event_init.has_key("oldURL") ? event_init.at("oldURL").to<ext::string>() : "";
    new_url = event_init.has_key("newURL") ? event_init.at("newURL").to<ext::string>() : "";
}


auto html::events::hash_change_event::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<hash_change_event>{isolate}
            .ctor<const ext::string&, const ext::string_any_map_t&>()
            .inherit<dom::events::event>()
            .var("oldURL", &hash_change_event::old_url)
            .var("newURL", &hash_change_event::new_url)
            .auto_wrap_objects();
}
