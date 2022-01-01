#include "event_listening.hpp"

#include <ext/types/typedefs.hpp>

#include <dom/aborting/abort_signal.hpp>

#include <dom/events/event.hpp>

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/shadows.hpp>
#include <dom/helpers/trees.hpp>

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


bool
dom::helpers::event_listening::flatten(
        std::variant<bool, ext::string_any_map> options) {

    return std::holds_alternative<bool>(options)
            ? std::get<bool>(options)
            : (bool)std::get<ext::string_any_map>(options).at("capture");
}


void
dom::helpers::event_listening::add_event_listener(
        nodes::event_target* event_target,
        ext::string_any_map& event_listener) {

    auto* signal = (aborting::abort_signal*)event_listener.at("signal");

    if (event_listener.at("callback").empty()) return;
    if (signal and signal->aborted) return;
    if (event_target->m_event_listeners.contains(event_listener)) return;

    event_target->m_event_listeners.append(event_listener);

    if (signal)
        signal->m_abort_algorithms.append([&event_listener, event_target] {remove_event_listener(event_target, event_listener);});
}


void
dom::helpers::event_listening::remove_event_listener(
        nodes::event_target* event_target,
        ext::string_any_map& event_listener) {

    using callback_t = std::function<void()>;

    event_listener.at("removed") = true;
    event_target->m_event_listeners.remove_if([event_listener](ext::cstring_any_map& existing_listener) {
        return (callback_t)existing_listener.at("callback") == (callback_t)event_listener.at("callback")
                and (ext::string)existing_listener.at("type") == (ext::string)event_listener.at("type")
                and (bool)existing_listener.at("capture") == (bool)event_listener.at("capture");
    });
}


void
dom::helpers::event_listening::remove_all_event_listeners(
        nodes::event_target* event_target) {

    for (ext::string_any_map& event_listener: event_target->m_event_listeners)
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
    nodes::node* parent_node = nullptr;

    if (event_target != related_target or event_target == event->related_target) {
        auto* node = dynamic_cast<nodes::node*>(event_target);

        ext::vector<nodes::event_target*> touch_targets;
        for (nodes::event_target* touch: *event->touch_targets)
            shadows::retarget(touch, event_target);

        event_dispatching::append_to_event_path(event, event_target, event_target, related_target, touch_targets, false);
        bool is_activation_event = event->type == "click";
        bool slot_in_closed_tree = false;

        if (is_activation_event)
            activation_target = event_target;
        if (shadows::is_slottable(node) and shadows::is_assigned(node))
            slottable = event_target;

        parent = event_target->get_the_parent(event);
        parent_node = dynamic_cast<nodes::node*>(parent);

        while (parent) {
            if (slottable) {
                assert(shadows::is_slot(parent_node));
                slottable = nullptr;
                slot_in_closed_tree = shadows::is_shadow_root(parent_node) and dynamic_cast<nodes::shadow_root*>(trees::root(parent_node))->mode == "closed";
            }

            if (shadows::is_slottable(node) and shadows::is_assigned(node))
                slottable = parent;

            related_target = shadows::retarget(event->related_target, parent);
            touch_targets= *event->touch_targets;
            for (nodes::event_target* touch: touch_targets)
                shadows::retarget(touch, parent);

            if (dynamic_cast<nodes::window*>(parent) or parent_node and shadows::is_shadow_including_ancestor(trees::root(node), parent_node)) {
                activation_target = is_activation_event and not activation_target and event->bubbles
                        ? event_target
                        : activation_target;
                event_dispatching::append_to_event_path(event, parent, nullptr, related_target, touch_targets, slot_in_closed_tree);
            }

            else if (parent == related_target)
                parent = nullptr;

            else {
                event_target = parent;
                activation_target = is_activation_event and not activation_target
                        ? event_target
                        : activation_target;
                event_dispatching::append_to_event_path(event, parent, event_target, related_target, touch_targets, slot_in_closed_tree);
            }

            slot_in_closed_tree = false;
            parent = parent
                    ? parent->get_the_parent(event)
                    : parent;
        }

        clear_targets_struct = *event->path
                ->filter([](auto* event_path_struct) {return event_path_struct->shadow_adjusted_target;})
                .back();

        clear_targets = not ext::vector<nodes::event_target*>{clear_targets_struct.shadow_adjusted_target, clear_targets_struct.related_target}
                .extend(clear_targets_struct.touch_targets)
                .cast_all<nodes::node*>()
                .filter([](nodes::node* node) {return shadows::is_shadow_root(node);})
                .empty();

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
    event->path->clear();

    event->m_dispatch_flag = false;
    event->m_stop_propagation_flag = false;
    event->m_stop_immediate_propagation_flag = false;

    if (clear_targets) {
        event->target = nullptr;
        event->related_target = nullptr;
        event->touch_targets->clear();
    }

    if (activation_target and not event->m_canceled_flag)
        ; // TODO : activation behaviour

    return not event->m_canceled_flag;
}

