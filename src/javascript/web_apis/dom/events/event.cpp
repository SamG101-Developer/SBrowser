#include "event.hpp"

#include <dom/helpers/event_listening.hpp>


/*
 * https://dom.spec.whatwg.org/#dom-event-event
 * https://developer.mozilla.org/en-US/docs/Web/API/Event/Event
 *
 * The Event() constructor creates a new Event object. An event created in this way is called a synthetic event, as
 * opposed to an event fired by the browser, and can be dispatched by a script.
 */
dom::events::event::event() : dom_object()
{
    // set the flag attributes
}


dom::events::event::event(
        ext::cstring& event_type,
        ext::cstring_any_map& event_init)

        : type(event_type)
        , bubbles(event_init.at("bubbles").to<bool>())
        , cancelable(event_init.at("bubbles").to<bool>())
        , composed(event_init.at("composed").to<bool>())
{
    // set the properties
    event_phase = NONE;
    touch_targets = std::make_unique<ext::vector<nodes::event_target*>>().get();
    path = std::make_unique<ext::vector<internal::event_path_struct*>>().get();
}


dom::events::event::~event()
{
    // clear the pointer lists
    touch_targets->clear();
    path->clear();
}


/*
 * https://dom.spec.whatwg.org/#dom-event-stoppropagation
 * https://developer.mozilla.org/en-US/docs/Web/API/Event/stopPropagation
 *
 * The stopPropagation() method of the Event interface prevents further propagation of the current event in the
 * capturing and bubbling phases. It does not, however, prevent any default behaviors from occurring; for instance,
 * clicks on links are still processed. If you want to stop those behaviors, see the preventDefault() method. It also
 * does not prevent immediate propagation to other event-handlers. If you want to stop those, see
 * stopImmediatePropagation().
 */
auto dom::events::event::stop_propagation() -> void
{
    // set the stop propagation flag, to stop the event propagating to the next target
    m_stop_propagation_flag = true;
}


/*
 * https://dom.spec.whatwg.org/#dom-event-stopimmediatepropagation
 * https://developer.mozilla.org/en-US/docs/Web/API/Event/stopImmediatePropagation
 *
 * The stopImmediatePropagation() method of the Event interface prevents other listeners of the same event from being
 * called. If several listeners are attached to the same element for the same event type, they are called in the order
 * in which they were added. If stopImmediatePropagation() is invoked during one such call, no remaining listeners will
 * be called.
 */
auto dom::events::event::stop_immediate_propagation() -> void
{
    // set the stop immediate propagation flag, to stop the event propagating to the next listener
    m_stop_immediate_propagation_flag = true;
}


/*
 * https://dom.spec.whatwg.org/#dom-event-preventdefault
 * https://developer.mozilla.org/en-US/docs/Web/API/Event/preventDefault
 *
 * The preventDefault() method of the Event interface tells the user agent that if the event does not get explicitly
 * handled, its default action should not be taken as it normally would be. The event continues to propagate as usual,
 * unless one of its event listeners calls stopPropagation() or stopImmediatePropagation(), either of which terminates
 * propagation at once. As noted below, calling preventDefault() for a non-cancelable event, such as one dispatched via
 * EventTarget.dispatchEvent(), without specifying cancelable: true has no effect.
 */
auto dom::events::event::prevent_default() -> void
{
    // set the cancelled flag if the event is cancelled and isn't in a passive listener
    m_canceled_flag = cancelable and not m_in_passive_listener_flag;
}


/*
 * https://dom.spec.whatwg.org/#dom-event-composedpath
 * https://developer.mozilla.org/en-US/docs/Web/API/Event/composedPath
 *
 * The composedPath() method of the Event interface returns the event's path which is an array of the objects on which
 * listeners will be invoked. This does not include nodes in shadow trees if the shadow root was created with its
 * ShadowRoot.mode closed.
 */
auto dom::events::event::composed_path() const -> ext::vector<dom::nodes::event_target*>
{
    // create the default vectors, and return if the current event traversal path is empty
    ext::vector<nodes::event_target*> composed_path_vector{};
    ext::vector<internal::event_path_struct*> path_vector{};
    if (path_vector.empty()) return composed_path_vector;

    composed_path_vector.append(current_target);

    // create the default indexing variables for node identification in the tree
    auto current_target_hidden_subtree_level = 1;
    auto current_target_index = 0;
    auto current_hidden_level = 0;
    auto max_hidden_level = 0;
    auto index = path_vector.length() - 1;

    // loop from the end of the event traversal path to identify the hidden subtree level of the current target
    while (index >= 0)
    {
        if (path_vector.at(index)->root_of_closed_tree) ++current_target_hidden_subtree_level;
        if (path_vector.at(index)->invocation_target == current_target) {current_target_index = (int)index; break;}
        if (path_vector.at(index)->slot_in_closed_tree) --current_target_hidden_subtree_level;
        --index;
    }

    // set the current and max hidden subtree level to the target's hidden subtree level, and the index to the target
    current_hidden_level = current_target_hidden_subtree_level;
    max_hidden_level = current_target_hidden_subtree_level;
    index = current_target_index - 1;

    // loop from the target to the beginning of the event path, prepending nodes that are in less hidden subtree levels
    while (index >= 0)
    {
        if (path_vector.at(index)->root_of_closed_tree) ++current_hidden_level;
        if (current_hidden_level <= max_hidden_level) composed_path_vector.prepend(path_vector.at(index)->invocation_target);
        if (path_vector.at(index)->slot_in_closed_tree)
        {
            --current_hidden_level;
            max_hidden_level = std::min(max_hidden_level, current_hidden_level);
        }
        --index;
    }

    // set the current and max hidden subtree level to the target's hidden subtree level, and the index to the target
    current_hidden_level = current_target_hidden_subtree_level;
    max_hidden_level = current_target_hidden_subtree_level;
    index = current_target_index - 1;

    // loop from the target to the end of the event path, appending nodes that are in less hidden subtree levels
    while (index < path_vector.length())
    {
        if (path_vector.at(index)->slot_in_closed_tree) ++current_hidden_level;
        if (current_hidden_level <= max_hidden_level) composed_path_vector.append(path_vector.at(index)->invocation_target);
        if (path_vector.at(index)->slot_in_closed_tree)
        {
            --current_hidden_level;
            max_hidden_level = std::min(max_hidden_level, current_hidden_level);
        }
        ++index;
    }

    // return the generated composed path
    return composed_path_vector;
}


auto dom::events::event::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<event>{isolate}
            .ctor<ext::cstring&, ext::cstring_any_map&>()
            .inherit<dom_object>()
            .static_("NONE", event::NONE)
            .static_("CAPTURING_PHASE", event::CAPTURING_PHASE)
            .static_("AT_TARGET", event::AT_TARGET)
            .static_("BUBBLING_PHASE", event::BUBBLING_PHASE)
            .function("stopImmediatePropagation", &event::stop_immediate_propagation)
            .function("stopPropagation", &event::stop_propagation)
            .function("preventDefault", &event::prevent_default)
            .function("composedPath", &event::composed_path)
            .var("type", &event::type)
            .var("bubbles", &event::bubbles)
            .var("cancelable", &event::cancelable)
            .var("composed", &event::composed)
            .var("target", &event::target)
            .var("currentTarget", &event::current_target)
            .var("relatedTarget", &event::related_target)
            .var("eventPhase", &event::event_phase)
            .var("timeStamp", &event::time_stamp)
            .var("isTrusted", &event::is_trusted)
            .var("touchTargets", &event::touch_targets)
            .var("path", &event::path)
            .auto_wrap_objects();
}
