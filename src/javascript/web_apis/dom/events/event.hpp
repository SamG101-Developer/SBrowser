#pragma once
#ifndef SBROWSER_EVENT_HPP
#define SBROWSER_EVENT_HPP

#include <ext/iterables/map.hpp>
#include <ext/iterables/string.hpp>
#include <ext/macros/decorators.hpp>
#include <ext/types/property.hpp>

#include <dom_object.hpp>

namespace dom {
    namespace events {class event;}
    namespace helpers {
        struct event_dispatching;
        struct event_listening;
    }
    namespace internal {struct event_path_struct;}
    namespace nodes {class event_target;}
}


class dom::events::event : public dom_object {
friends
    friend class dom::nodes::event_target;
    friend struct dom::helpers::event_dispatching;
    friend struct dom::helpers::event_listening;

public: constructors
    event(ext::cstring& event_type, ext::cstring_any_map& event_init = {});
    ~event() override;

public: static_constants
    static constexpr unsigned char NONE = 0;
    static constexpr unsigned char CAPTURING_PHASE = 1;
    static constexpr unsigned char AT_TARGET = 2;
    static constexpr unsigned char BUBBLING_PHASE = 3;

public: methods
    void stop_propagation();
    void stop_immediate_propagation();
    void prevent_default();
    ext::vector<nodes::event_target*> composed_path();

public: properties
    ext::dom_property<ext::string> type;
    ext::dom_property<bool> bubbles;
    ext::dom_property<bool> cancelable;
    ext::dom_property<bool> composed;
    ext::dom_property<nodes::event_target*> target;
    ext::dom_property<nodes::event_target*> current_target;
    ext::dom_property<nodes::event_target*> related_target;
    ext::dom_property<unsigned short> event_phase;
    ext::dom_property<double> time_stamp;
    ext::dom_property<bool> is_trusted;
    ext::dom_property<ext::vector<nodes::event_target*>*> touch_targets;
    ext::dom_property<ext::vector<internal::event_path_struct*>*> path;

public: internal_methods
    ext::any v8(v8::Isolate *isolate) const override;

protected: internal_properties
    bool m_stop_propagation_flag;
    bool m_stop_immediate_propagation_flag;
    bool m_canceled_flag;
    bool m_in_passive_listener_flag;
    bool m_initialized_flag;
    bool m_dispatch_flag;
};


#endif //SBROWSER_EVENT_HPP
