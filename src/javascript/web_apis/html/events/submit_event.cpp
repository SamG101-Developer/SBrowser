#include "submit_event.hpp"

#include <html/elements/html_element.hpp>


html::events::submit_event::submit_event() = default;


html::events::submit_event::submit_event(
        const ext::string& event_type,
        const ext::string_any_map_t& event_init)

        : dom::events::event(event_type, event_init)
        , submitter(event_init.at("submitter").to<elements::html_element*>())
{
}


auto html::events::submit_event::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<submit_event>{isolate}
            .ctor<const ext::string&, const ext::string_any_map_t&>()
            .inherit<dom::events::event>()
            .var("submitter", &submit_event::submitter)
            .auto_wrap_objects();
}
