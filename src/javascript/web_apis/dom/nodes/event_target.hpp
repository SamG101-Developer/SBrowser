#pragma once
#ifndef SBROWSER_EVENT_TARGET_HPP
#define SBROWSER_EVENT_TARGET_HPP

#include <functional>

#include <ext/map.hpp>
#include <ext/string.hpp>
#include <ext/vector.hpp>
#include <ext/decorators.hpp>
#include <dom_object.hpp>

namespace dom::events {class event;}
namespace dom::helpers {struct event_dispatching;}
namespace dom::helpers {struct event_listening;}
namespace dom::nodes {class event_target;}


class dom::nodes::event_target : virtual public dom_object
{
public friends:
    friend struct helpers::event_dispatching;
    friend struct helpers::event_listening;
    using event_listener_callback = std::function<void()>;

public constructors:
    event_target() = default;
    ~event_target() override;

public methods:
    auto add_event_listener(ext::string type, event_listener_callback&& callback, const ext::string_any_map& options) -> void;
    auto remove_event_listener(ext::string type, event_listener_callback&& callback, const ext::string_any_map& options) -> void;
    auto dispatch_event(events::event* event) -> bool;

public internal_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected internal_methods:
    virtual auto get_the_parent(events::event* event) -> event_target*;

private internal_properties:
    ext::vector<ext::string_any_map> m_event_listeners;
};


#endif //SBROWSER_EVENT_TARGET_HPP
