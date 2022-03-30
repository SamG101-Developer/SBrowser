#pragma once
#ifndef SBROWSER_EVENT_HPP
#define SBROWSER_EVENT_HPP

#include <dom_object.hpp>
#include <ext/map.hpp>
#include <ext/decorators.hpp>
#include <ext/dom_property.hpp>
#include <performance/time/dom_high_res_timestamp.hpp>

#include <veque.hpp>

namespace dom::events {class event;}
namespace dom::helpers {struct event_dispatching;}
namespace dom::helpers {struct event_listening;}
namespace dom::internal {struct event_path_struct;}
namespace dom::nodes {class event_target;}


class dom::events::event : virtual public dom_object
{
public friends:
    friend class dom::nodes::event_target;
    friend struct dom::helpers::event_dispatching;
    friend struct dom::helpers::event_listening;

public constructors:
    event();
    event(const ext::string& event_type, const ext::string_any_map& event_init = {});

    ~event() override;

public static_constants:
    static constexpr unsigned char NONE = 0;
    static constexpr unsigned char CAPTURING_PHASE = 1;
    static constexpr unsigned char AT_TARGET = 2;
    static constexpr unsigned char BUBBLING_PHASE = 3;

public js_methods:
    auto stop_propagation() -> void;
    auto stop_immediate_propagation() -> void;
    auto prevent_default() -> void;

    new_obj auto composed_path() const -> ext::vector<nodes::event_target*>;

public js_properties:
    ext::dom_property<ext::string> type;
    ext::dom_property<bool> bubbles;
    ext::dom_property<bool> cancelable;
    ext::dom_property<bool> composed;

    ext::dom_property<nodes::event_target*> target = nullptr;
    ext::dom_property<nodes::event_target*> current_target = nullptr;
    ext::dom_property<nodes::event_target*> related_target = nullptr;

    ext::dom_property<unsigned short> event_phase;
    ext::dom_property<double> time_stamp = performance::time::dom_high_res_timestamp();
    ext::dom_property<bool> is_trusted;

    ext::dom_property<ext::vector<nodes::event_target        *>*> touch_targets;
    ext::dom_property<ext::vector<internal::event_path_struct*>*> path;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    bool m_stop_propagation_flag = false;
    bool m_stop_immediate_propagation_flag = false;
    bool m_canceled_flag = false;
    bool m_in_passive_listener_flag = false;
    bool m_initialized_flag = true;
    bool m_dispatch_flag = false;
};


#endif //SBROWSER_EVENT_HPP
