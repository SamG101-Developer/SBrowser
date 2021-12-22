#include "event.hpp"

#include <dom/helpers/event_listening.hpp>


dom::events::event::event(ext::cstring& event_type, ext::cstring_any_map& event_init)
        : type(event_type)
        , bubbles(ext::any_cast<bool>(event_init.at("bubbles")))
        , cancelable(ext::any_cast<bool>(event_init.at("cancelable")))
        , composed(ext::any_cast<bool>(event_init.at("composed")))
        , event_phase(NONE)
        , time_stamp(performance::time::dom_high_res_timestamp())
        , touch_targets(new ext::vector<nodes::event_target*>{})
        , path(new ext::vector<internal::event_path_struct*>{})
        {

    target = nullptr;
    current_target = nullptr;
    related_target = nullptr;

    m_initialized_flag = true;
}


dom::events::event::~event() {
    delete touch_targets;
    delete path;
}


void
dom::events::event::stop_propagation() {
    m_stop_propagation_flag = true;
}


void
dom::events::event::stop_immediate_propagation() {
    m_stop_immediate_propagation_flag = true;
}


void
dom::events::event::prevent_default() {
    m_canceled_flag = cancelable and not m_in_passive_listener_flag;
}


ext::vector<dom::nodes::event_target*>
dom::events::event::composed_path() {

    ext::vector<nodes::event_target*> composed_path_vector{};
    ext::vector<internal::event_path_struct*> path_vector{};
    if (path_vector.empty()) return composed_path_vector;

    composed_path_vector.append(current_target);

    auto current_target_hidden_subtree_level = 1;
    auto current_target_index                = 0;
    auto current_hidden_level                = 0;
    auto max_hidden_level                    = 0;
    auto index = path_vector.length() - 1;

    while (index >= 0) {
        if (path_vector.at(index)->root_of_closed_tree) ++current_target_hidden_subtree_level;
        if (path_vector.at(index)->invocation_target == current_target) {current_target_index = index; break;}
        if (path_vector.at(index)->slot_in_closed_tree) --current_target_hidden_subtree_level;
        --index;
    }

    current_hidden_level = current_target_hidden_subtree_level;
    max_hidden_level     = current_target_hidden_subtree_level;
    index                = current_target_index - 1;

    while (index >= 0) {
        if (path_vector.at(index)->root_of_closed_tree) ++current_hidden_level;
        if (current_hidden_level <= max_hidden_level) composed_path_vector.prepend(path_vector.at(index)->invocation_target);
        if (path_vector.at(index)->slot_in_closed_tree) {
            --current_hidden_level;
            max_hidden_level = std::min(max_hidden_level, current_hidden_level);
        }
        --index;
    }

    current_hidden_level = current_target_hidden_subtree_level;
    max_hidden_level     = current_target_hidden_subtree_level;
    index                = current_target_index - 1;

    while (index < path_vector.length()) {
        if (path_vector.at(index)->slot_in_closed_tree) ++current_hidden_level;
        if (current_hidden_level <= max_hidden_level) composed_path_vector.append(path_vector.at(index)->invocation_target);
        if (path_vector.at(index)->slot_in_closed_tree) {
            --current_hidden_level;
            max_hidden_level = std::min(max_hidden_level, current_hidden_level);
        }
        ++index;
    }

    return composed_path_vector;
}
