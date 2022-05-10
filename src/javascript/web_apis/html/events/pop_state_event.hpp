#pragma once
#ifndef SBROWSER_POP_STATE_EVENT_HPP
#define SBROWSER_POP_STATE_EVENT_HPP

#include <dom/events/event.hpp>

namespace html::events {class pop_state_event;}


class html::events::pop_state_event : public dom::events::event
{
public constructors:
    pop_state_event() = default;
    pop_state_event(const ext::string& event_type, const ext::string_any_map_t& event_init = {});

public js_properties:
    ext::html_property<ext::any, _F> state;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_POP_STATE_EVENT_HPP
