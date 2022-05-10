#include "event_dispatching.hpp"

#include <functional>

#include <javascript/environment/realms.hpp>

#include <dom/events/event.hpp>

#include <dom/helpers/event_listening.hpp>
#include <dom/helpers/shadows.hpp>

#include <dom/nodes/element.hpp>
#include <dom/nodes/node.hpp>
#include <dom/nodes/shadow_root.hpp>
#include <dom/nodes/window.hpp>

#include <html/events/drag_event.hpp>
#include <html/events/page_transition_event.hpp>

#include <html/helpers/dragging_internals.hpp>

#include <html/dragging/data_transfer.hpp>

#include <QtGui/QGuiApplication>


auto dom::helpers::event_dispatching::append_to_event_path(
        const events::event* const event,
        nodes::event_target* const invocation_target,
        nodes::event_target* const shadow_adjusted_target,
        nodes::event_target* const related_target,
        const ext::vector<const nodes::event_target*>& touch_targets,
        const bool slot_in_closed_tree)
        -> void
{
    // get the invocation target (cast as a shadow root), and if the invocation target is in the shadow tree or the root
    // of a closed tree
    const auto* const invocation_target_as_shadow_root = dynamic_cast<const nodes::shadow_root*>(invocation_target);
    const bool invocation_target_in_shadow_tree = shadows::is_root_shadow_root(dynamic_cast<const nodes::node*>(invocation_target));
    const bool root_of_closed_tree =
            shadows::is_shadow_root(dynamic_cast<const nodes::node*>(invocation_target))
            and invocation_target_as_shadow_root
            and invocation_target_as_shadow_root->mode == ext::string{"closed"};

    // add a new struct into the event path traversal list
    event->path->append(new internal::event_path_struct
    {
        .invocation_target = invocation_target,
        .shadow_adjusted_target = shadow_adjusted_target,
        .related_target = related_target,
        .touch_targets = touch_targets,
        .invocation_target_in_shadow_tree = invocation_target_in_shadow_tree,
        .root_of_closed_tree = root_of_closed_tree,
        .slot_in_closed_tree = slot_in_closed_tree
    });
}


auto dom::helpers::event_dispatching::invoke(
        internal::event_path_struct* const event_path_struct,
        events::event* const event,
        const unsigned char phase)
        -> void
{
    // create a copy of the event path, upto the event path struct being invoked
    ext::vector<internal::event_path_struct*> temp_vector{
        event->path->begin(),
        std::find(event->path->begin(), event->path->end(), event_path_struct)
    };

    // set the related target, touch targets and the target to the corresponding values from the event path struct - the
    // target is first target in the event path list that has a shadow adjusted target
    event->related_target = event_path_struct->related_target;
    event->touch_targets = &event_path_struct->touch_targets;
    event->target = temp_vector.front()->shadow_adjusted_target;

    // return if the stop propagation flag has been set (then stop propagating through the event path list)
    if (event->m_stop_propagation_flag)
        return;

    // set the event's current target to the invocation target from the event path struct being processed
    event->current_target = event_path_struct->invocation_target;

    // invoke a copy of the event listeners
    const auto& listeners {event->current_target->m_event_listeners};
    inner_invoke(event, listeners, phase);
}


auto dom::helpers::event_dispatching::inner_invoke(
        events::event* const event,
        const ext::vector<ext::string_any_map_t>& event_listeners,
        const unsigned char phase)
        -> void
{
    // create a callback_t for casting
    using callback_t = std::function<void()>;

    // iterator over each event listener
    for (const auto& event_listener: event_listeners)
    {
        // continue if the event listener type doesn't match the event type
        if (event->type != event_listener.at("type").to<ext::string>())
            continue;

        // continue if the event is capturing and the event listener isn't
        if (phase == events::event::CAPTURING_PHASE and not event_listener.at("capturing").to<bool>())
            continue;

        // continue if the event is bubbling and the event listener isn't
        if (phase == events::event::BUBBLING_PHASE and not event_listener.at("bubbling").to<bool>())
            continue;

        // set the event's is_passive_listener_flag to the value stored by the event listener and invoke the callback
        // method in the event listener - reset the is_passive_listener_flag after the invocation
        event->m_in_passive_listener_flag = event_listener.at("passive").to<bool>();
        event_listener.at("callback").to<callback_t>()();
        event->m_in_passive_listener_flag = false;

        // if the listener was a one-time callback then remove it
        if (event_listener.at("once").to<bool>())
            event->current_target->m_event_listeners.remove(event_listener);

        // return if the stop immediate propagation flag has been set (stop propagating through the listeners)
        if (event->m_stop_immediate_propagation_flag)
            return;
    }
}


template <typename T>
auto dom::helpers::event_dispatching::fire_event(
        const ext::string& e,
        nodes::event_target* target,
        const ext::string_any_map_t& init)
        -> bool // TODO : concept
{
    // create a new event of type T and dispatch it through the event paths
    auto* event = new T{e, init};
    return event_listening::dispatch(event, target);
}


auto dom::helpers::event_dispatching::fire_synthetic_pointer_event(
        const ext::string& e,
        const nodes::event_target* const target,
        const bool not_trusted_flag)
        -> bool
{

//    auto* event = new pointer_events::events::pointer_event{e, {{"bubbles", true}, {"cancelable", true}, {"composed", true}}};
//    event->is_trusted = not not_trusted_flag;
//    event->ctrl_key = QGuiApplication::keyboardModifiers() & Qt::KeyboardModifier::ControlModifier;
//    event->shift_key = QGuiApplication::keyboardModifiers() & Qt::KeyboardModifier::ShiftModifier;
//    event->alt_key = QGuiApplication::keyboardModifiers() & Qt::KeyboardModifier::AltModifier;
//    event->meta_key = QGuiApplication::keyboardModifiers() & Qt::KeyboardModifier::MetaModifier;
    // TODO -> event->view

//    return event_listening::dispatch(event, target);
    return true; // TODO
}


auto dom::helpers::event_dispatching::fire_drag_and_drop_event(
        const ext::string& e,
        nodes::element* target,
        html::internal::drag_data_store* drag_data_store,
        nodes::event_target* related_target) -> bool
{
    auto data_drag_store_was_changed = false;

    auto window = javascript::realms::relevant_realm(target->owner_document);
    if (e == "dragstart")
    {
        drag_data_store->drag_data_store_mode = html::helpers::dragging_internals::drag_data_store_mode_t::READ_WRITE;
        data_drag_store_was_changed = true;
    }
    else if (e == "drop")
        drag_data_store->drag_data_store_mode = html::helpers::dragging_internals::drag_data_store_mode_t::READ_ONLY;

    auto* data_transfer = new html::dragging::data_transfer{};
    data_transfer->m_drag_data_store = drag_data_store;
    data_transfer->effect_allowed = drag_data_store->drag_data_store_allowed_effects_state;

    if (ext::string_vector{"dragstart", "drag", "dragleave"}.contains(e))
        data_transfer->drop_effect = "none";
    else if (ext::string_vector{"drop", "dragend"}.contains(e))
        data_transfer->drop_effect = data_transfer->m_current_drag_operation;
    else
        data_transfer->drop_effect = data_transfer->effect_allowed; // TODO : map value

    auto* event = new html::events::drag_event{e, {
        {"bubbles", true},
        {"view", window},
        {"relatedTarget", related_target},
        {"dataTransfer", data_transfer},
        {"cancelable", not ext::string_vector{"dragleave", "dragend"}.contains(e)}}};

    // TODO : mouse and key attributes (in map above)

    auto result = event_listening::dispatch(event, target);
    drag_data_store->drag_data_store_allowed_effects_state = data_transfer->effect_allowed;
    if (data_drag_store_was_changed)
        drag_data_store->drag_data_store_mode = html::helpers::dragging_internals::drag_data_store_mode_t::PROTECTED;
    data_transfer->m_drag_data_store = nullptr;
}


auto dom::helpers::event_dispatching::fire_page_transition_event(
        const ext::string& e,
        nodes::window* target,
        const bool persisted)
        -> bool
{
    auto* event = new html::events::page_transition_event{e, {{"persisted", persisted}, {"cancelable", true}, {"bubbles", true}}};
    return event_listening::dispatch(event, target);
}
