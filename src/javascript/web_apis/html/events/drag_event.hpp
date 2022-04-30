#pragma once
#ifndef SBROWSER_DRAG_EVENT_HPP
#define SBROWSER_DRAG_EVENT_HPP

#include <ui_events/events/mouse_event>

namespace html::events {class drag_event;}

#include <html/dragging/data_transfer.hpp>


class html::events::drag_event : public ui_events::events::mouse_event
{
public constructors:
    drag_event(const ext::string& event_type, const ext::string_any_map& event_init = {});

public js_properties:
    html::dragging::data_transfer* data_transfer = nullptr;
};


#endif //SBROWSER_DRAG_EVENT_HPP
