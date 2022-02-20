#include "custom_event.hpp"


dom::events::custom_event::custom_event(
        ext::cstring& event_type,
        ext::cstring_any_map& event_init)

        : dom::events::event(event_type, event_init)
{
    // set the properties
    detail = event_init.at("detail");
}


auto dom::events::custom_event::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<custom_event>{isolate}
            .ctor<ext::cstring&, ext::cstring_any_map&>()
            .inherit<event>()
            .var("detail", &custom_event::detail)
            .auto_wrap_objects();
}
