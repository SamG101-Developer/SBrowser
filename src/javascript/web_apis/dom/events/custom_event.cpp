#include "custom_event.hpp"


dom::events::custom_event::custom_event(
        const ext::string& event_type,
        const ext::string_any_map& event_init)

        : event(event_type, event_init)
        , detail(event_init.at("detail"))
{
}


auto dom::events::custom_event::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<custom_event>{isolate}
            .ctor<const ext::string&, const ext::string_any_map&>()
            .inherit<event>()
            .var("detail", &custom_event::detail)
            .auto_wrap_objects();
}
