#include "event_listening.hpp"

#include <dom/aborting/abort_signal.hpp>
#include <dom/events/event.hpp>
#include <dom/helpers/event_dispatching.hpp>
#include <dom/nodes/node.hpp>
#include <dom/nodes/window.hpp>
#include <dom/nodes/shadow_root.hpp>


ext::string_any_map
dom::helpers::event_listening::flatten_more(
        std::variant<bool, ext::string_any_map> options) {

    return std::holds_alternative<bool>(options)
           ? ext::string_any_map{{"capture", std::get<bool>(options)}}
           : std::get<ext::string_any_map>(options);
}


ext::string_any_map
dom::helpers::event_listening::flatten(
        std::variant<bool, ext::string_any_map> options) {

    return std::holds_alternative<bool>(options)
            ? std::get<bool>(options)
            : ext::any_cast<bool>(std::get<ext::string_any_map>(options).at("capture"));
}


void
dom::helpers::event_listening::add_event_listener(
        nodes::event_target* event_target,
        ext::string_any_map& event_listener) {

    auto signal = ext::any_cast<aborting::abort_signal*>(event_listener.at("signal"));

    if (not event_listener.at("callback").has_value()) return;
    if (signal and signal->aborted) return;
    if (event_target->m_event_listeners.contains(event_listener)) return;

    event_target->m_event_listeners.append(event_listener);
    if (signal) signal->m_abort_algorithms.append([event_listener, event_target] -> void {
        remove_event_listener(event_target, event_listener);
    });
}


void
dom::helpers::event_listening::remove_event_listener(
        nodes::event_target* event_target,
        ext::string_any_map& event_listener) {

    event_listener.at("removed").emplace<bool>(true);
    event_target->m_event_listeners.remove_if([event_listener](ext::cstring_any_map& existing_listener) -> bool {
        return existing_listener.at("callback") == event_listener.at("callback")
                and existing_listener.at("type") == event_listener.at("type")
                and existing_listener.at("capture") == event_listener.at("capture");
    });
}


void
dom::helpers::event_listening::remove_all_event_listeners(
        nodes::event_target* event_target) {

    for (const ext::string_any_map& event_listener: event_target->m_event_listeners)
        remove_event_listener(event_target, event_listener);
}


bool
dom::helpers::event_listening::dispatch(
        events::event* event,
        nodes::event_target* event_target) {

    event->m_dispatch_flag = true;
    internal::event_path_struct clear_targets_struct{};
    bool clear_targets = false;

    nodes::event_target* activation_target = nullptr;
    nodes::event_target* slottable = nullptr;
    nodes::event_target* parent = nullptr;
    nodes::event_target* related_target = shadows::retarget(event->related_target, event_target);

    if (event_target != related_target or event_target == event->related_target) {
        ext::vector<nodes::event_target*> touch_targets;
        for (nodes::event_target* touch: *event->touch_targets)
            shadows::retarget(touch, event_target);

        event_dispatching::append_to_event_path(event, event_target, event_target, related_target, touch_targets, false);
        bool is_activation_event = event->type == "click";
        bool slot_in_closed_tree = false;

        if (is_activation_event)
            activation_target = event_target;
        if (shadows::is_slottable(event_target) and shadows::is_assigned(event_target))
            slottable = event_target;

        parent = event_target->get_the_parent(event);

        while (parent) {
            if (slottable) {
                assert(shadows::is_slot(parent));
                slottable = nullptr;
                slot_in_closed_tree = shadows::is_shadow_root(parent) and dynamic_cast<nodes::shadow_root*>(trees::root(parent))->mode == "closed";
            }

            if (shadows::is_slottable(event_target) and shadows::is_assigned(event_target))
                slottable = parent;

            related_target = shadows::retarget(event->related_target, parent);
            touch_targets  = event->touch_targets;
            for (nodes::event_target* touch: touch_targets)
                shadows::retarget(touch, parent);

            if (dynamic_cast<nodes::window*>(parent) or dynamic_cast<nodes::node*>(parent) and shadows::is_shadow_including_ancestor(trees::root(event_target), parent)) {
                if (is_activation_event and not activation_target and event->bubbles)
                    activation_target = event_target;
                event_dispatching::append_to_event_path(event, parent, nullptr, related_target, touch_targets, slot_in_closed_tree);
            }
            else if (parent == related_target) {
                parent = related_target;
            }
            else (event_target = parent) {
                if (is_activation_event and not activation_target)
                    activation_target = event_target;
                event_dispatching::append_to_event_path(event, parent, event_target, related_target, touch_targets, slot_in_closed_tree);
            }

            if (parent) parent = parent->get_the_parent(event);
            slot_in_closed_tree = false;
        }

        clear_targets_struct = event->path
                ->filter([](auto* event_path_struct) -> bool {return event_path_struct.shadow_adjusted_target;})
                .back();

        clear_targets = not ext::vector<nodes::event_target*>{clear_targets_struct.shadow_adjusted_target, clear_targets_struct.related_target}
                .extend(clear_targets_struct.touch_targets)
                .filter([](auto* event_target) -> bool {shadows::is_shadow_root(event_target);})
                .readonly_setter();

        for (auto* event_path_struct: *event->path) {
            event->event_phase = event_path_struct->shadow_adjusted_target ? events::event::AT_TARGET : events::event::CAPTURING_PHASE;
            event_dispatching::invoke(event_path_struct, event, events::event::CAPTURING_PHASE);
        }

        for (auto* event_path_struct: *event->path) {
            event->event_phase = event_path_struct->shadow_adjusted_target ? events::event::AT_TARGET : events::event::BUBBLING_PHASE;
            event_dispatching::invoke(event_path_struct, event, events::event::BUBBLING_PHASE);
        }
    }

    event->event_phase = events::event::NONE;
    event->current_target = nullptr;
    event->path.clear();

    event->m_dispatch_flag = false;
    event->m_stop_propagation_flag = false;
    event->m_stop_immediate_propagation_flag = false;

    if (clear_targets) {
        event->target = nullptr;
        event->related_target = nullptr;
        event->touch_targets->clear()
    }

    if (activation_target and not event->m_canceled_flag)
        ; // TODO : activation behaviour

    return not event->m_canceled_flag;
}

