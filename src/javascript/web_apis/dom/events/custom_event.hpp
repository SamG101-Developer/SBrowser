#pragma once
#ifndef SBROWSER_CUSTOM_EVENT_HPP
#define SBROWSER_CUSTOM_EVENT_HPP

#include <ext/macros/decorators.hpp>
#include <ext/properties/dom_property.hpp>
#include <dom/events/event.hpp>

namespace dom::events {class custom_event;}


/*
 * https://dom.spec.whatwg.org/#interface-customevent
 * https://developer.mozilla.org/en-US/docs/Web/API/CustomEvent
 *
 * The CustomEvent interface represents events initialized by an application for any purpose.
 */
class dom::events::custom_event : public event
{
public constructors:
    using event::event;
    custom_event(const ext::string& event_type, const ext::string_any_map& event_init = {});

public properties:
    ext::dom_property<ext::any, _F> detail;

public internal_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CUSTOM_EVENT_HPP
