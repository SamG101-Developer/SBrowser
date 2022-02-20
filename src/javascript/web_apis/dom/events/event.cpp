#include "event.hpp"

#include <dom/helpers/event_listening.hpp>

#include <performance/time/dom_high_res_timestamp.hpp>


dom::events::event::event() : dom_object()
{
    // set the flag attributes
    m_stop_propagation_flag = false;
    m_stop_immediate_propagation_flag = false;
    m_canceled_flag = false;
    m_in_passive_listener_flag = false;
    m_initialized_flag = true;
    m_dispatch_flag = false;
}


dom::events::event::event(
        ext::cstring& event_type,
        ext::cstring_any_map& event_init):

        event()
{
    // set the attributes from the options dictionary
    type = event_type;
    bubbles = event_init.at("bubbles").to<bool>();
    cancelable = event_init.at("cancelable").to<bool>();
    composed = event_init.at("composed").to<bool>();
    target = nullptr;
    current_target = nullptr;
    related_target = nullptr;
    event_phase = NONE;
    time_stamp = performance::time::dom_high_res_timestamp();
    touch_targets = new ext::vector<nodes::event_target*>{};
    path = new ext::vector<internal::event_path_struct*>{};
}


dom::events::event::~event()
{
    // clear the pointer lists
    delete touch_targets;
    delete path;
}


auto dom::events::event::stop_propagation() -> void
{
    // set the stop propagation flag so that the recursive method to find the next node will stop
    m_stop_propagation_flag = true;
}


auto dom::events::event::stop_immediate_propagation() -> void
{
    // set the stop immediate propagation flat so that the recursive method to find the next listener will stop
    m_stop_immediate_propagation_flag = true;
}


auto dom::events::event::prevent_default() -> void
{
    // set the cancelled flag, as long as the event is cancelable and not in a passive listener
    m_canceled_flag = cancelable and not m_in_passive_listener_flag;
}


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
        if (path_vector.at(index)->invocation_target == current_target) {current_target_index = index; break;}
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
