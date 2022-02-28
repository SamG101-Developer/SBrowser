#include "event_dispatching.hpp"

#include <functional>

#include <dom/events/event.hpp>

#include <dom/helpers/event_listening.hpp>
#include <dom/helpers/shadows.hpp>

#include <dom/nodes/node.hpp>
#include <dom/nodes/shadow_root.hpp>

#include <QtGui/QGuiApplication>


auto dom::helpers::event_dispatching::append_to_event_path(
        events::event* event,
        nodes::event_target* invocation_target,
        nodes::event_target* shadow_adjusted_target,
        nodes::event_target* related_target,
        ext::cvector<const nodes::event_target*>& touch_targets,
        const bool slot_in_closed_tree)
        -> void
{
    // get the invocation target (cast as a shadow root), and if the invocation target is in the shadow tree or the root
    // of a closed tree
    auto* invocation_target_as_shadow_root = dynamic_cast<const nodes::shadow_root*>(invocation_target);
    bool invocation_target_in_shadow_tree = shadows::is_root_shadow_root(dynamic_cast<const nodes::node*>(invocation_target));
    bool root_of_closed_tree =
            shadows::is_shadow_root(dynamic_cast<const nodes::node*>(invocation_target))
            and invocation_target_as_shadow_root
            and invocation_target_as_shadow_root->mode == ext::string{"closed"};

    // add a new struct into the event path traversal list
    event->path->append(std::make_unique<internal::event_path_struct>
    (
        .invocation_target = invocation_target,
        .shadow_adjusted_target = shadow_adjusted_target,
        .related_target = related_target,
        .touch_targets = touch_targets,
        .invocation_target_in_shadow_tree = invocation_target_in_shadow_tree,
        .root_of_closed_tree = root_of_closed_tree,
        .slot_in_closed_tree = slot_in_closed_tree
    ));
}


auto dom::helpers::event_dispatching::invoke(
        internal::event_path_struct* event_path_struct,
        events::event* event,
        unsigned char phase)
        -> void
{
    // create a copy of the event path, upto the event path struct being invoked
    ext::vector<internal::event_path_struct*> temp_vector {
        event->path->begin(),
        event->path->begin() + event->path->find(event_path_struct)
    };

    // set the related target, touch targets and the target to the corresponding values from the event path struct - the
    // target is first target in the event path list that has a shadow adjusted target
    event->related_target = event_path_struct->related_target;
    event->touch_targets = &event_path_struct->touch_targets;
    event->target = temp_vector
            .transform<nodes::event_target*>([](internal::event_path_struct* struct_) {return struct_->shadow_adjusted_target;})
            .front();

    // return if the stop propagation flag has been set (then stop propagating through the event path list)
    if (event->m_stop_propagation_flag)
        return;

    // set the event's current target to the invocation target from the event path struct being processed
    event->current_target = event_path_struct->invocation_target;

    // invoke a copy of the event listeners
    ext::vector<ext::string_any_map> listeners {event->current_target->m_event_listeners};
    inner_invoke(event, listeners, phase);
}


auto dom::helpers::event_dispatching::inner_invoke(
        events::event* event,
        ext::vector<ext::string_any_map>& event_listeners,
        unsigned char phase)
        -> void
{
    // create a callback_t for casting
    using callback_t = std::function<void()>;

    // iterator over each event listener
    for (auto& event_listener: event_listeners)
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
        ext::cstring& e,
        nodes::event_target* target,
        ext::cstring_any_map& init)
        -> bool
{
    // create a new event of type T and dispatch it through the event paths
    T* event = new T{e, init};
    event_listening::dispatch(event, target);
}


auto dom::helpers::event_dispatching::fire_synthetic_pointer_event(
        ext::cstring& e,
        nodes::event_target* target,
        bool not_trusted_flag)
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
