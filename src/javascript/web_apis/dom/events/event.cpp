#include "event.hpp"

#include <dom/helpers/event_listening.hpp>


dom::events::event::event() = default;


dom::events::event::event(
        const ext::string& event_type,
        const ext::string_any_map& event_init)

        : type(event_type)
        , bubbles(event_init.at("bubbles").to<bool>())
        , cancelable(event_init.at("bubbles").to<bool>())
        , composed(event_init.at("composed").to<bool>())
{
    // set the properties
    event_phase = NONE;
    touch_targets = new ext::vector<nodes::event_target*>{};
    path = new ext::vector<internal::event_path_struct*>{};
}


dom::events::event::~event()
{
    // clear the pointer lists
    touch_targets->clear();
    path->clear();
}


auto dom::events::event::stop_propagation(
        ) -> void
{
    // set the stop propagation flag, to stop the event propagating to the next target
    m_stop_propagation_flag = true;
}


auto dom::events::event::stop_immediate_propagation(
        ) -> void
{
    // set the stop immediate propagation flag, to stop the event propagating to the next listener
    m_stop_immediate_propagation_flag = true;
}


auto dom::events::event::prevent_default(
        ) -> void
{
    // set the cancelled flag if the event is cancelled and isn't in a passive listener
    m_canceled_flag = cancelable and not m_in_passive_listener_flag;
}


auto dom::events::event::composed_path(
        ) const -> ext::vector<dom::nodes::event_target*>
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


auto dom::events::event::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<event>{isolate}
            .template ctor<const ext::string&, const ext::string_any_map&>()
            .template inherit<dom_object>()
            .template static_("NONE", event::NONE, true)
            .template static_("CAPTURING_PHASE", event::CAPTURING_PHASE, true)
            .template static_("AT_TARGET", event::AT_TARGET, true)
            .template static_("BUBBLING_PHASE", event::BUBBLING_PHASE, true)
            .template function("stopImmediatePropagation", &event::stop_immediate_propagation)
            .template function("stopPropagation", &event::stop_propagation)
            .template function("preventDefault", &event::prevent_default)
            .template function("composedPath", &event::composed_path)
            .template var("type", &event::type, true)
            .template var("bubbles", &event::bubbles, true)
            .template var("cancelable", &event::cancelable, true)
            .template var("composed", &event::composed, true)
            .template var("target", &event::target, true)
            .template var("currentTarget", &event::current_target, true)
            .template var("relatedTarget", &event::related_target, true)
            .template var("eventPhase", &event::event_phase, true)
            .template var("timeStamp", &event::time_stamp, true)
            .template var("isTrusted", &event::is_trusted, true)
            .template var("touchTargets", &event::touch_targets, true)
            .template var("path", &event::path, true)
            .auto_wrap_objects();
}
