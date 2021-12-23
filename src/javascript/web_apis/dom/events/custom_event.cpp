#include "custom_event.hpp"


dom::events::custom_event::custom_event(ext::cstring& event_type, ext::cstring_any_map& event_init)
        : dom::events::event(event_type, event_init) {

    detail = event_init.at("detail");
}
