#include "form_data_event.hpp"


html::events::form_data_event::form_data_event() = default;


html::events::form_data_event::form_data_event(
        const ext::string& event_type,
        const ext::string_any_map_t& event_init)

        : dom::events::event(event_type, event_init)
        , form_data(event_init.at("formData").to<xhr::form_data*>())
{
}


auto html::events::form_data_event::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<form_data_event>{isolate}
            .ctor<const ext::string&, const ext::string_any_map_t&>()
            .inherit<dom::events::event>()
            .var("formData", &form_data_event::form_data)
            .auto_wrap_objects();
}
