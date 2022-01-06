#include "event_dispatching.hpp"

#include <functional>

#include <dom/events/event.hpp>

#include <dom/helpers/event_listening.hpp>
#include <dom/helpers/shadows.hpp>

#include <dom/nodes/node.hpp>
#include <dom/nodes/shadow_root.hpp>

#include <QtGui/QGuiApplication>


void
dom::helpers::event_dispatching::append_to_event_path(
        events::event* event,
        nodes::event_target* invocation_target,
        nodes::event_target* shadow_adjusted_target,
        nodes::event_target* related_target,
        ext::vector<nodes::event_target*>& touch_targets,
        bool slot_in_closed_tree) {

    auto* invocation_target_as_shadow_root = dynamic_cast<nodes::shadow_root*>(invocation_target);
    bool invocation_target_in_shadow_tree = shadows::is_root_shadow_root(dynamic_cast<nodes::node*>(invocation_target));
    bool root_of_closed_tree =
            shadows::is_shadow_root(dynamic_cast<nodes::node*>(invocation_target))
            and invocation_target_as_shadow_root
            and invocation_target_as_shadow_root->mode == "closed";

    event->path->append(new internal::event_path_struct {
        .invocation_target = invocation_target,
        .shadow_adjusted_target = shadow_adjusted_target,
        .related_target = related_target,
        .touch_targets = touch_targets,
        .invocation_target_in_shadow_tree = invocation_target_in_shadow_tree,
        .root_of_closed_tree = root_of_closed_tree,
        .slot_in_closed_tree = slot_in_closed_tree
    });
}


void
dom::helpers::event_dispatching::invoke(
        internal::event_path_struct* event_path_struct,
        events::event* event,
        unsigned char phase) {

    ext::vector<internal::event_path_struct*> temp_vector {
        event->path->begin(),
        event->path->begin() + event->path->find(event_path_struct)
    };

    event->related_target = event_path_struct->related_target;
    event->touch_targets = &event_path_struct->touch_targets;
    event->target = temp_vector
            .transform<nodes::event_target*>([](internal::event_path_struct* struct_) -> nodes::event_target* {return struct_->shadow_adjusted_target;})
            .front();

    if (event->m_stop_propagation_flag)
        return;

    event->current_target = event_path_struct->invocation_target;
    ext::vector<ext::string_any_map> listeners {event->current_target->m_event_listeners};
    inner_invoke(event, listeners, phase);
}


void
dom::helpers::event_dispatching::inner_invoke(
        events::event* event,
        ext::vector<ext::string_any_map>& event_listeners,
        unsigned char phase) {

    using callback_t = std::function<void()>;

    for (auto& event_listener: event_listeners) {
        if (event->type != event_listener.at("type").to<ext::string>()) continue;
        if (phase == events::event::CAPTURING_PHASE and not event_listener.at("capturing").to<bool>()) continue;
        if (phase == events::event::BUBBLING_PHASE and not event_listener.at("bubbling").to<bool>()) continue;

        event->m_in_passive_listener_flag = event_listener.at("passive").to<bool>();
        event_listener.at("callback").to<callback_t>()();
        event->m_in_passive_listener_flag = false;

        if (event_listener.at("once").to<bool>())
            event->current_target->m_event_listeners.remove(event_listener);

        if (event->m_stop_immediate_propagation_flag)
            return;
    }
}


template <typename T>
bool
dom::helpers::event_dispatching::fire_event(
        ext::cstring& e,
        nodes::event_target* target,
        ext::cstring_any_map& init) {

    T* event = new T{e, init};
    event_listening::dispatch(event, target);
}


bool
dom::helpers::event_dispatching::fire_synthetic_pointer_event(
        ext::cstring& e,
        nodes::event_target* target,
        bool not_trusted_flag) {

//    auto* event = new pointer_events::events::pointer_event{e, {{"bubbles", true}, {"cancelable", true}, {"composed", true}}};
//    event->is_trusted = not not_trusted_flag;
//    event->ctrl_key = QGuiApplication::keyboardModifiers() & Qt::KeyboardModifier::ControlModifier;
//    event->shift_key = QGuiApplication::keyboardModifiers() & Qt::KeyboardModifier::ShiftModifier;
//    event->alt_key = QGuiApplication::keyboardModifiers() & Qt::KeyboardModifier::AltModifier;
//    event->meta_key = QGuiApplication::keyboardModifiers() & Qt::KeyboardModifier::MetaModifier;
    // TODO -> event->view

//    return event_listening::dispatch(event, target);
}
