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


auto dom::helpers::event_listening::flatten_more(std::variant<bool, ext::string_any_map> options) -> ext::string_any_map
{
    // return {capture: true} if the options is a bool value, otherwise the map already being held in the variant
    return std::holds_alternative<bool>(options)
           ? ext::string_any_map{{std::make_pair("capture", std::get<bool>(options))}}
           : std::get<ext::string_any_map>(options);
}


auto dom::helpers::event_listening::flatten(std::variant<bool, ext::string_any_map> options) -> bool
{
    // return the boolean if a boolean value is being stored in the variant, otherwise the capture option of the map
    return std::holds_alternative<bool>(options)
            ? std::get<bool>(options)
            : std::get<ext::string_any_map>(options).at("capture").to<bool>();
}


auto dom::helpers::event_listening::add_event_listener(
        nodes::event_target* event_target,
        ext::string_any_map& event_listener)
        -> void
{
    // get the abort signal from the event listener
    auto* signal = event_listener.at("signal").to<aborting::abort_signal*>();

    // return if there is no callback as invoking the event listener would have no effect, and would waste cycles
    if (event_listener.at("callback").empty())
        return;

    // return if there is a signal, and it has already aborted, as there is no need for the event listener to exist
    if (signal and signal->aborted)
        return;

    // return if the event listener is already stored in the event listeners list - no duplicates allowed
    if (event_target->m_event_listeners.contains(event_listener))
        return;

    // if all the other checks pass, then append the event listener to the event listeners list
    event_target->m_event_listeners.append(event_listener);

    // if there is an abort signal, then add an abort algorithm that removes itself from the event listeners list
    if (signal)
        signal->m_abort_algorithms.append([&event_listener, event_target] {remove_event_listener(event_target, event_listener);});
}


auto dom::helpers::event_listening::remove_event_listener(
        nodes::event_target* event_target,
        ext::string_any_map& event_listener)
        -> void
{
    // create a callback_t for casting
    using callback_t = std::function<void()>;

    // set the removed attribute to true
    event_listener.at("removed") = true;

    // remove all event listeners that have a matching callback, type and capture attribute to event_listener
    event_target->m_event_listeners.remove_if([event_listener](ext::cstring_any_map& existing_listener)
    {
        return existing_listener.at("callback").to<callback_t>() == event_listener.at("callback").to<callback_t>()
                and existing_listener.at("type").to<ext::string>() == event_listener.at("type").to<ext::string>()
                and existing_listener.at("capture").to<bool>() == event_listener.at("capture").to<bool>();
    });
}


auto dom::helpers::event_listening::remove_all_event_listeners(nodes::event_target* event_target) -> void
{
    // iterate over event listeners and remove them all
    for (ext::string_any_map& event_listener: event_target->m_event_listeners)
        remove_event_listener(event_target, event_listener);
}


auto dom::helpers::event_listening::dispatch(
        events::event* event,
        nodes::event_target* event_target)
        -> bool
{
    // set the dispatch_flag to true
    event->m_dispatch_flag = true;
    internal::event_path_struct* clear_targets_struct = nullptr;
    bool clear_targets = false;

    // set the related target to the event's related target retargeted against the event_target
    nodes::event_target* related_target = shadows::retarget(event->related_target, event_target);

    // if the event target isn't the retargeted related target or is the event's related target
    if (event_target != related_target or event_target == event->related_target) {

        // define the is_activation_event and slot_in_closed_tree
        bool is_activation_event = event->type == "click";
        bool slot_in_closed_tree = false;

        // set the touch target list to a retargeted transform of the event's touch target list, the node to a node
        // cast of the event target, the activation_target to the event_target if the event is an activation event, the
        // slottable to the event_target if it is an assigned slottable, the parent to event traversal parent algorithm
        // output, and the parent_node to a node cast of the parent
        auto touch_targets = event->touch_targets->transform<>([event_target](nodes::event_target* touch) {return shadows::retarget(touch, event_target);});
        auto* node = dynamic_cast<nodes::node*>(event_target);
        auto* activation_target = is_activation_event ? event_target : nullptr;
        auto* slottable = shadows::is_slottable(node) and shadows::is_assigned(node) ? event_target : nullptr;
        auto* parent = event_target->get_the_parent(event);
        auto* parent_node = dynamic_cast<nodes::node*>(parent);

        // append the current info into the event path
        event_dispatching::append_to_event_path(event, event_target, event_target, related_target, touch_targets, false);

        // while there is an available parent (not ->parent_node, but .get_the_parent() for event traversal)
        while (parent)
        {
            // if there is a slottable node then handle the shadow roots
            if (slottable)
            {
                // assert that the parent_node is a slot, reset the slottable, and set slot_in_closed_tree if the parent
                // is a closed shadow root
                assert(shadows::is_slot(parent_node));
                slottable = nullptr;
                slot_in_closed_tree = shadows::is_shadow_root(parent_node) and shadows::shadow_root(parent_node)->mode == "closed";
            }

            // set the touch target list, related target and slottable to what they were before, but using the parent
            // instead of the event target for retargeting / input to shadow helper algorithms
            touch_targets = event->touch_targets->transform<>([parent](nodes::event_target* touch) {return shadows::retarget(touch, parent);});
            related_target = shadows::retarget(event->related_target, parent);
            slottable = shadows::is_slottable(node) and shadows::is_assigned(node) ? parent : slottable;

            // if the parent is a window, or the parent is a node who is a shadow including ancestor of the parent_node,
            // then append the information to the event's path, with no shadow adjusted target
            if (dynamic_cast<nodes::window*>(parent) or parent_node and shadows::is_shadow_including_ancestor(trees::root(node), parent_node))
            {
                // set the activation target if there is an activation event, no current activation target, and the
                // event will bubble
                activation_target = is_activation_event and not activation_target and event->bubbles
                        ? event_target
                        : activation_target;

                event_dispatching::append_to_event_path(event, parent, nullptr, related_target, touch_targets, slot_in_closed_tree);
            }

            // otherwise, if the parent is the related target, then set the parent to nullptr, so the loop will break
            else if (parent == related_target)
                parent = nullptr;

            // otherwise, set the event_target to the parent, and append the information to the event's path, with the
            // shadow adjusted target set to the event_target (which is the parent)
            else
            {
                // set the event_target to the parent
                event_target = parent;

                // set the activation target if there is an activation event, no current activation target
                activation_target = is_activation_event and not activation_target
                        ? event_target
                        : activation_target;

                event_dispatching::append_to_event_path(event, parent, event_target, related_target, touch_targets, slot_in_closed_tree);
            }

            // reset the slot_in_closed_tree, and get the next parent with the event traversal get_the_parent method
            slot_in_closed_tree = false;
            if (parent)
                parent->get_the_parent(event);
        }

        // set the clear_targets_struct to the last struct in tht event's path that has a shadow_adjusted_target
        clear_targets_struct = event->path->last_match([](internal::event_path_struct* event_path_struct) -> bool {return event_path_struct->shadow_adjusted_target;});

        // clear the targets if the shadow_adjusted_target, related_target or any touch_target has a shadow root
        clear_targets = ext::vector<nodes::event_target*>{clear_targets_struct->shadow_adjusted_target, clear_targets_struct->related_target}
                .extend(clear_targets_struct->touch_targets)
                .cast_all<nodes::node*>()
                .filter([](nodes::node* node) {return shadows::is_shadow_root(node);});

        // capturing phase event invocation
        for (auto* event_path_struct: *event->path) {
            event->event_phase = event_path_struct->shadow_adjusted_target ? events::event::AT_TARGET : events::event::CAPTURING_PHASE;
            event_dispatching::invoke(event_path_struct, event, events::event::CAPTURING_PHASE);
        }

        // bubbling phase event invocation
        for (auto* event_path_struct: *event->path) {
            event->event_phase = event_path_struct->shadow_adjusted_target ? events::event::AT_TARGET : events::event::BUBBLING_PHASE;
            event_dispatching::invoke(event_path_struct, event, events::event::BUBBLING_PHASE);
        }
    }

    // set the phase to none, remove the current target and clear the event path
    event->event_phase = events::event::NONE;
    event->current_target = nullptr;
    event->path->clear();

    // set traversal flags to false
    event->m_dispatch_flag = false;
    event->m_stop_propagation_flag = false;
    event->m_stop_immediate_propagation_flag = false;

    // if the targets need clearing, remove the target and related target, and clear the touch targets
    if (clear_targets)
    {
        event->target = nullptr;
        event->related_target = nullptr;
        event->touch_targets->clear();
    }

    // TODO if (activation_target and not event->m_canceled_flag)
    // TODO : activation behaviour

    // return if the event hasn't been cancelled
    return not event->m_canceled_flag;
}

