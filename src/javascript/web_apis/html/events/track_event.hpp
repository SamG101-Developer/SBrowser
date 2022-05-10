#pragma once
#ifndef SBROWSER_TRACK_EVENT_HPP
#define SBROWSER_TRACK_EVENT_HPP

#include <dom/events/event.hpp>

namespace html::events {class track_event;}
namespace html::media {class abstract_track;}


class html::events::track_event : public dom::events::event
{
public constructors:
    track_event();
    track_event(const ext::string& event_type, const ext::string_any_map_t& event_init = {});

public js_properties:
    ext::dom_property<media::abstract_track*> track;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_TRACK_EVENT_HPP
