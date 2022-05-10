#include "track_event.hpp"


html::events::track_event::track_event() = default;


html::events::track_event::track_event(
        const ext::string& event_type,
        const ext::string_any_map_t& event_init)

        : dom::events::event(event_type, event_init)
        , track(event_init.at("track").to<media::abstract_track*>())
{
}


auto html::events::track_event::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<track_event>{isolate}
            .ctor<const ext::string&, const ext::string_any_map_t&>()
            .inherit<dom::events::event>()
            .var("track", &track_event::track)
            .auto_wrap_objects();
}
