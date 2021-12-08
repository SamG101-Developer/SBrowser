#pragma once
#ifndef SBROWSER_EVENT_LISTENING_HPP
#define SBROWSER_EVENT_LISTENING_HPP

#include <ext/any.hpp>
#include <ext/string.hpp>

namespace dom {
    namespace events {class event;}
    namespace helpers {struct event_listening;}
    namespace internal {struct event_path_struct;}
    namespace nodes {class event_target;}
}


struct dom::helpers::event_listening {
    static ext::string_any_map flatten_more(std::variant<bool, ext::string_any_map> options);
    static ext::string_any_map flatten (std::variant<bool, ext::string_any_map> options);
    static void add_event_listener(nodes::event_target* event_target, ext::string_any_map& event_listener);
    static void remove_event_listener(nodes::event_target* event_target, ext::string_any_map& event_listener);
    static void remove_all_event_listeners(nodes::event_target* event_target);
    static bool dispatch(events::event* event, nodes::event_target* event_target);
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
