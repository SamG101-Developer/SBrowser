#pragma once
#ifndef SBROWSER_CUSTOM_EVENT_HPP
#define SBROWSER_CUSTOM_EVENT_HPP

#include <ext/decorators.hpp>
#include <ext/dom_property.hpp>
#include <dom/events/event.hpp>

namespace dom::events {class custom_event;}


class dom::events::custom_event final : public event
{
public constructors:
    using event::event;
    custom_event(const ext::string& event_type, const ext::string_any_map& event_init = {});

public properties:
    ext::dom_property<ext::any, _F> detail;

public internal_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_CUSTOM_EVENT_HPP
