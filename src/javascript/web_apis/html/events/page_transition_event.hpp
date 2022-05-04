#pragma once
#ifndef SBROWSER_PAGE_TRANSITION_EVENT_HPP
#define SBROWSER_PAGE_TRANSITION_EVENT_HPP

#include <dom/events/event.hpp>

namespace html::events {class page_transition_event;}


class html::events::page_transition_event : public dom::events::event
{
public constructors:
    page_transition_event() = default;
    page_transition_event(const ext::string& event_type, const ext::string_any_map& event_init = {});

public js_properties:
    ext::html_property<bool, _F> persisted;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_PAGE_TRANSITION_EVENT_HPP
