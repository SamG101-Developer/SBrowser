#pragma once
#ifndef SBROWSER_HASH_CHANGE_EVENT_HPP
#define SBROWSER_HASH_CHANGE_EVENT_HPP

#include <dom/events/event.hpp>

namespace html::events {class hash_change_event;}


class html::events::hash_change_event : public dom::events::event
{
public constructors:
    hash_change_event() = default;
    hash_change_event(const ext::string& event_type, const ext::string_any_map& event_init = {});

public js_properties:
    ext::html_property<ext::string, _F> old_url;
    ext::html_property<ext::string, _F> new_url;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HASH_CHANGE_EVENT_HPP
