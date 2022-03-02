#pragma once
#ifndef SBROWSER_EVENT_DISPATCHING_HPP
#define SBROWSER_EVENT_DISPATCHING_HPP

#include <ext/iterables/map.hpp>
#include <ext/iterables/vector.hpp>
#include <ext/iterables/string.hpp>

namespace dom
{
    namespace events {class event;}
    namespace helpers {struct event_dispatching;}
    namespace internal {struct event_path_struct;}
    namespace nodes {class event_target;}
}


/*
 * Group of helper methods designed to help with dispatching events through the dom tree, including the invoking of
 * events in different targets and the multiple event handlers in each target. Event firing is also managed from here,
 * with template-based construction for the different event types.
 */
struct dom::helpers::event_dispatching
{
    static auto append_to_event_path(const events::event* event, nodes::event_target* invocation_target, nodes::event_target* shadow_adjusted_target, nodes::event_target* related_target, ext::cvector<const nodes::event_target*>& touch_targets, const bool slot_in_closed_tree) -> void;
    static auto invoke(internal::event_path_struct* event_path_struct, events::event* event, unsigned char phase) -> void;
    static auto inner_invoke(events::event* event, ext::cvector<ext::string_any_map>& event_listeners, unsigned char phase) -> void;

    template <class T=events::event> static auto fire_event(ext::cstring& e, nodes::event_target* target, ext::cstring_any_map& init = {}) -> bool;
    static auto fire_synthetic_pointer_event(ext::cstring& e, const nodes::event_target* target, bool not_trusted_flag = false) -> bool;
};


#endif //SBROWSER_EVENT_DISPATCHING_HPP
