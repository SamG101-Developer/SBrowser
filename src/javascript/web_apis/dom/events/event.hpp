#pragma once
#ifndef SBROWSER_EVENT_HPP
#define SBROWSER_EVENT_HPP

#include <dom_object.hpp>

namespace dom::events {class event;}

#include <ext/map.hpp>
#include <ext/decorators.hpp>
#include <performance/time/dom_high_res_timestamp.hpp>

#include <veque.hpp>

namespace dom::helpers  {struct event_dispatching;}
namespace dom::helpers  {struct event_listening;}
namespace dom::internal {struct event_path_struct;}
namespace dom::nodes    {class event_target;}


class dom::events::event : public virtual dom_object
{
private aliases:
    using touch_targets_t = ext::vector<nodes::event_target*>;
    using path_t          = ext::vector<internal::event_path_struct*>;

public friends:
    friend class dom::nodes::event_target;
    friend struct dom::helpers::event_dispatching;
    friend struct dom::helpers::event_listening;

public constructors:
    event();
    explicit event(const ext::string& event_type, const ext::string_any_map_t& event_init = {});

public js_static_constants:
    static constexpr unsigned short NONE = 0;
    static constexpr unsigned short CAPTURING_PHASE = 1;
    static constexpr unsigned short AT_TARGET = 2;
    static constexpr unsigned short BUBBLING_PHASE = 3;

public js_methods:
    auto stop_propagation() -> void;
    auto stop_immediate_propagation() -> void;
    auto prevent_default() -> void;

    [[nodiscard]] new_obj auto composed_path() const -> ext::vector<nodes::event_target*>;

public js_properties:
    ext::property<ext::string> type;
    ext::property<bool> bubbles;
    ext::property<bool> cancelable;
    ext::property<bool> composed;

    ext::property<smart_pointer<nodes::event_target>> target;
    ext::property<smart_pointer<nodes::event_target>> current_target;
    ext::property<smart_pointer<nodes::event_target>> related_target;

    ext::property<ushort> event_phase;
    ext::property<double> time_stamp;
    ext::property<bool> is_trusted;

    ext::property<smart_pointer<touch_targets_t>> touch_targets;
    ext::property<smart_pointer<path_t>>          path;

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
