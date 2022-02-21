#pragma once
#ifndef SBROWSER_EVENT_LISTENING_HPP
#define SBROWSER_EVENT_LISTENING_HPP

#include <ext/iterables/map.hpp>
#include <ext/iterables/string.hpp>

namespace dom {
    namespace events {class event;}
    namespace helpers {struct event_listening;}
    namespace internal {struct event_path_struct;}
    namespace nodes {class event_target;}
}


struct dom::helpers::event_listening {
    static auto flatten_more(std::variant<bool, ext::string_any_map> options) -> ext::string_any_map;
    static auto flatten (std::variant<bool, ext::string_any_map> options) -> bool;
    static auto add_event_listener(nodes::event_target* event_target, ext::string_any_map& event_listener) -> void;
    static auto remove_event_listener(nodes::event_target* event_target, ext::string_any_map& event_listener) -> void;
    static auto remove_all_event_listeners(nodes::event_target* event_target) -> void;
    static auto dispatch(events::event* node, nodes::event_target* event_target) -> bool;
};


struct dom::internal::event_path_struct {
    nodes::event_target* invocation_target = nullptr;
    nodes::event_target* shadow_adjusted_target = nullptr;
    nodes::event_target* related_target = nullptr;

    ext::vector<nodes::event_target*> touch_targets;
    bool invocation_target_in_shadow_tree;
    bool root_of_closed_tree;
    bool slot_in_closed_tree;

    inline bool operator==(const event_path_struct& other) {return this == &other;}
};


#endif //SBROWSER_EVENT_LISTENING_HPP
