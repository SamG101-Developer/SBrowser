#pragma once
#ifndef SBROWSER_EVENT_DISPATCHING_HPP
#define SBROWSER_EVENT_DISPATCHING_HPP

#include <ext/map.hpp>
#include <ext/any.hpp>

namespace dom::events {class event;}
namespace dom::helpers {struct event_dispatching;}
namespace dom::internal {struct event_path_struct;}
namespace dom::nodes {class event_target;}
namespace dom::nodes {class element;}
namespace dom::nodes {class window;}
namespace html::internal {struct drag_data_store;}


struct dom::helpers::event_dispatching final
{
    // event appending
    static auto append_to_event_path(
            const events::event* event,
            nodes::event_target* invocation_target,
            nodes::event_target* shadow_adjusted_target,
            nodes::event_target* related_target,
            const ext::vector<const nodes::event_target*>& touch_targets,
            bool slot_in_closed_tree)
            -> void;

    // event invocation
    static auto invoke(
            internal::event_path_struct* event_path_struct,
            events::event* event,
            unsigned char phase)
            -> void;

    static auto inner_invoke(
            events::event* event,
            const ext::vector<ext::string_any_map>& event_listeners,
            unsigned char phase)
            -> void;

    // event firing
    template <class T=events::event>
    static auto fire_event(
            const ext::string& e,
            nodes::event_target* target,
            const ext::string_any_map& init = {})
            -> bool;

    static auto fire_synthetic_pointer_event(
            const ext::string& e,
            const nodes::event_target* target,
            bool not_trusted_flag = false)
            -> bool;

    static auto fire_drag_and_drop_event(
            const ext::string& e,
            nodes::element* target,
            html::internal::drag_data_store* drag_data_store,
            nodes::event_target* related_target)
            -> bool;

    static auto fire_page_transition_event(
            const ext::string& e,
            nodes::window* target,
            bool persisted)
            -> bool;
};


#endif //SBROWSER_EVENT_DISPATCHING_HPP
