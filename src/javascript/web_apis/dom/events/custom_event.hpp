#pragma once
#ifndef SBROWSER_CUSTOM_EVENT_HPP
#define SBROWSER_CUSTOM_EVENT_HPP

#include <ext/iterables//map.hpp>
#include <ext/macros/decorators.hpp>
#include <ext/types/property.hpp>
#include <dom/events/event.hpp>

namespace dom::events {class custom_event;}


class dom::events::custom_event : public event {

public constructors:
    custom_event(ext::cstring& event_type, ext::cstring_any_map& event_init = {});

public properties:
    ext::dom_property<ext::any> detail;
};


#endif //SBROWSER_CUSTOM_EVENT_HPP
