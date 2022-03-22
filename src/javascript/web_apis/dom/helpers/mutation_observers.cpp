#include "mutation_observers.hpp"

#include <queue>

#include <ext/map.hpp>
#include <ext/set.hpp>

#include <javascript/environment/realms.hpp>

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/mutations/mutation_observer.hpp>
#include <dom/mutations/mutation_record.hpp>

#include <dom/nodes/node.hpp>

#include <html/elements/html_slot_element.hpp>
#include <html/elements/html_media_element.hpp>

#include <v8.h>


auto dom::helpers::mutation_observers::notify_mutation_observers()
        -> void
{
    // set the mutation_observer_microtask_queue object in javascript to false
    javascript::realms::surrounding_agent().set("mutation_observer_microtask_queue", false);

    // get the notification and signal-slots set from the javascript context
    const auto& notify_set = javascript::realms::surrounding_agent().get<ext::set<mutations::mutation_observer*>&>("mutation_observers");
    const auto& signal_slots_set = javascript::realms::surrounding_agent().get<ext::set<html::elements::html_slot_element*>&>("signal_slots");

    // clear the signal-slots set in javascript
    javascript::realms::surrounding_agent().set<decltype(signal_slots_set)>("signal_slots", {});

    //
    for (auto* const mutation_observer: notify_set)
    {
        // create a new records list and fill it from the mutation observers record queue, clearing it in the process
        auto* const records = new ext::vector<mutations::mutation_record*>{};
        for (size_t index = 0; index < mutation_observer->m_record_queue->size(); ++index)
        {
            records->append(mutation_observer->m_record_queue->front());
            mutation_observer->m_record_queue->pop();
        }

        // for each node in the mutation observer's node list, remove the node's registered observers if they are the
        // same observer as this
        for (auto* const node: *mutation_observer->m_node_list)
            node->m_registered_observer_list->remove_if([mutation_observer](auto* observer) {return observer->observer == mutation_observer;});

        // if the records list isn't empty, TODO
        if (not records->empty())
        {
            const v8::TryCatch exception_handler{v8::Isolate::GetCurrent()};
            mutation_observer->m_callback(records, mutation_observer);

            if (exception_handler.HasCaught())
                ; // TODO : console::reporting::report_warning_to_console(exception_handler.Message()->Get());
        }
    }

    for (auto* const slot: signal_slots_set)
        event_dispatching::fire_event<>("slotChange", slot, ext::string_any_map{{"bubbles", true}});
}


auto dom::helpers::mutation_observers::queue_microtask(
        steps_t&& steps)
        -> void
{
    /* TODO */
}


auto dom::helpers::mutation_observers::queue_mutation_record(
        const ext::string& type,
        const nodes::event_target* const target,
        const ext::string& name,
        const ext::string& namespace_,
        const ext::string& old_value,
        const ext::vector<nodes::node*>& added_nodes,
        const ext::vector<nodes::node*>& removed_nodes,
        nodes::node* const previous_sibling,
        nodes::node* const next_sibling)
        -> void
{
    ext::cmap<mutations::mutation_observer*, ext::string> interested_observers;
    const ext::vector<nodes::node*> nodes = trees::ancestors((const nodes::node* const)(target));

    for (auto* const node: nodes) {
        for (auto* const registered: *node->m_registered_observer_list) {
            const auto& options = registered->options;
            const auto attribute_filter = options.at("attributeFilter").to<ext::vector<ext::string>>();

            if (not ((node != target and not options.at("subtree").to<bool>()))
                    or (type == "attributes" and not options.at("attributes").to<bool>())
                    or (type == "attributes" and not attribute_filter.empty() and (not attribute_filter.contains(name) or not namespace_.empty()))
                    or (type == "characterData" and not options.at("characterData").to<bool>())
                    or (type == "childList" and not options.at("childList").to<bool>())) {

                mutations::mutation_observer* const mutation_observer = registered->observer;

                if (not interested_observers.has_key(mutation_observer))
                    interested_observers.at(mutation_observer) = "";

                if (type == "attributes" and options.at("attributeOldValue").to<bool>() or type == "characterData" and options.at("characterDataOldValue").to<bool>())
                    interested_observers.at(mutation_observer) = old_value;
            }
        }
    }

    for (const auto& [observer, mapped_old_value]: interested_observers) {
        auto mutation_record = new mutations::mutation_record{};
        mutation_record->type = type;
        mutation_record->attribute_name = name;
        mutation_record->attribute_namespace = namespace_;
        mutation_record->old_value = mapped_old_value;
        mutation_record->target = (nodes::node* const)target;
        mutation_record->previous_sibling = previous_sibling;
        mutation_record->next_sibling = next_sibling;
        mutation_record->added_nodes = added_nodes;
        mutation_record->removed_nodes = removed_nodes;

        observer->m_record_queue->push(mutation_record);
    }

    mutation_observers::queue_mutation_observers_microtask();
}


auto dom::helpers::mutation_observers::queue_tree_mutation_record(
        nodes::event_target* const target,
        const ext::vector<nodes::node*>& added_nodes,
        const ext::vector<nodes::node*>& removed_nodes,
        nodes::node* const previous_sibling,
        nodes::node* const next_sibling)
        -> void
{
    queue_mutation_record("childList", target, "", "", "", added_nodes, removed_nodes, previous_sibling, next_sibling);
}


auto dom::helpers::mutation_observers::queue_mutation_observers_microtask()
        -> void
{
    queue_microtask(&notify_mutation_observers);
}


auto dom::helpers::mutation_observers::queue_media_element_task(
        html::elements::html_media_element* element,
        steps_t&& steps)
        -> void
{
    // queue_element_task(element->element_event_task_source, element, steps);
}
