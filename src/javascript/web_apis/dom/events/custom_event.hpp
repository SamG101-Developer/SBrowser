#pragma once
#ifndef SBROWSER_CUSTOM_EVENT_HPP
#define SBROWSER_CUSTOM_EVENT_HPP

#include <ext/iterables/map.hpp>
#include <ext/macros/decorators.hpp>
#include <ext/properties/dom_property.hpp>
#include <dom/events/event.hpp>

namespace dom::events {class custom_event;}


class dom::events::custom_event : public event {

public: constructors
    custom_event(): event() {};
    custom_event(ext::cstring& event_type, ext::cstring_any_map& event_init = {});

public: properties
    ext::dom_property<ext::any, _F> detail;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CUSTOM_EVENT_HPP
