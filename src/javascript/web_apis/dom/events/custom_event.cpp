#include "custom_event.hpp"


/*
 * https://dom.spec.whatwg.org/#dom-customevent-customevent
 * https://developer.mozilla.org/en-US/docs/Web/API/CustomEvent/CustomEvent
 *
 * The CustomEvent() constructor creates a new CustomEvent.
 */
dom::events::custom_event::custom_event(
        ext::cstring& event_type,
        ext::cstring_any_map& event_init)

        : dom::events::event(event_type, event_init)
        , detail(event_init.at("detail"))
{
}


auto dom::events::custom_event::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<custom_event>{isolate}
            .ctor<ext::cstring&, ext::cstring_any_map&>()
            .inherit<event>()
            .var("detail", &custom_event::detail)
            .auto_wrap_objects();
}
