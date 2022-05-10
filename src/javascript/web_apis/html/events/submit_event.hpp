#pragma once
#ifndef SBROWSER_SUBMIT_EVENT_HPP
#define SBROWSER_SUBMIT_EVENT_HPP

#include <dom/events/event.hpp>

namespace html::events {class submit_event;}
namespace html::elements {class html_element;}


class html::events::submit_event : public dom::events::event
{
public constructors:
    submit_event();
    submit_event(const ext::string& event_type, const ext::string_any_map_t& event_init = {});

public js_properties:
    ext::dom_property<elements::html_element*, _F> submitter;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_SUBMIT_EVENT_HPP
