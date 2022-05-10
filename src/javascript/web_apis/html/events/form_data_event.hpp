#pragma once
#ifndef SBROWSER_FORM_DATA_EVENT_HPP
#define SBROWSER_FORM_DATA_EVENT_HPP

#include <dom/events/event.hpp>

namespace html::events {class form_data_event;}
namespace xhr {class form_data;}


class html::events::form_data_event : public dom::events::event
{
public constructors:
    form_data_event();
    form_data_event(const ext::string& event_type, const ext::string_any_map_t& event_init = {});

public js_properties:
    ext::dom_property<xhr::form_data*, _F> form_data;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_FORM_DATA_EVENT_HPP
