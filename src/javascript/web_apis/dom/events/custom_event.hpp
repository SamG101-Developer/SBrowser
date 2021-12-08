#pragma once
#ifndef SBROWSER_CUSTOM_EVENT_HPP
#define SBROWSER_CUSTOM_EVENT_HPP

#include <any>

#include <ext/decorators.hpp>
#include <dom/events/event.hpp>

namespace dom::events {class custom_event;}


class dom::events::custom_event
        : public dom::events::event {

public constructors:
    custom_event(ext::cstring& event_type, ext::cstring_any_map& event_init = {});

public properties:
    ext::dom_property<std::any> detail;
};


#endif //SBROWSER_CUSTOM_EVENT_HPP
