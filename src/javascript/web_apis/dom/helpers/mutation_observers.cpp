#include "mutation_observers.hpp"

#include <unordered_set>
#include <queue>

#include <ext/map.hpp>

#include <dom/helpers/event_dispatching.hpp>
#include <dom/nodes/node.hpp>

#include <v8.h>


void dom::helpers::mutation_observers::notify_mutation_observers() {

    javascript::realms::surrounding_agent().set("mutation_observer_microtask_queue", false);
    std::unordered_set<mutations::mutation_observer*> notify_set = javascript::realms::surrounding_agent().get("mutation_observers");
    std::unordered_set<html::elements::html_slot_element*> signal_slots_set = javascript::realms::surrounding_agent().get("signal_slots");
    javascript::realms::surrounding_agent().get("signal_slots").clear(true);

    for (auto* mutation_observer: notify_set) {
        std::queue<mutations::mutation_record*> records {*mutation_observer->m_record_queue};
        mutation_observer->m_record_queue = {};

        for (auto* node: *mutation_observer->m_node_list)
            node->m_registered_observer_list.remove_if([mutation_observer](auto* observer) {return observer->observer == mutation_observer;});

        if (not records.empty()) {
            v8::TryCatch exception_handler{v8::Isolate::GetCurrent()};
            mutation_observer->callback(records, mutation_observer);

            if (exception_handler.HasCaught()) console::reporting::report_warning_to_console(exception_handler.Message()->Get());
        }
    }

    for (auto* slot: signal_slots_set)
        event_dispatching::fire_event<>("slotChange", slot, ext::cstring_any_map{{"bubbles", true}});
}


void dom::helpers::mutation_observers::queue_microtask(steps_t&& steps) {

    /* TODO */
}


void
dom::helpers::mutation_observers::queue_mutation_record(
        ext::cstring& type,
        nodes::event_target* target,
        ext::cstring& name,
        ext::cstring& namespace_,
        ext::cstring& old_value,
        ext::vector<nodes::node*>& added_nodes,
        ext::vector<nodes::node*>& removed_nodes,
        nodes::node* previous_sibling,
        nodes::node* next_sibling) {

    ext::map<mutations::mutation_record*, ext::string> interested_observers;
    ext::vector<nodes::node*> nodes = trees::ancestors(target);

    for (auto* node: nodes) {
        for (auto* registered: *node->m_registered_observer_list) {
            auto options = registered->options;

            if (not ((node != target and options.at("subtree") == false))
                    or (type == "attributes" and not options.at("attributes"))
                    or (type == "attributes" and options.at("attributeFilter") and (not options.at("attributeFilter").contains(name) or not namespace_))
                    or (type == "characterData" and not options.at("characterData"))
                    or (type == "childList" and not options.at("childList"))) {

                mutations::mutation_observer* mutation_observer = registered->observer;

                if (not interested_observers.contains(mutation_observer))
                    interested_observers.at(mutation_observer) = nullptr;

                if ((type == "attributes" and options.at("attributeOldValue") or type == "characterData" and options.at("characterDataOldValue")))
                    interested_observers.at(mutation_observer) = old_value;
            }
        }
    }

    for (const auto& [observer, mapped_old_value]: interested_observers) {
        observer->m_record_queue.push(mutations::mutation_record{
            .type = type,
            .target = dynamic_cast<nodes::node*>(target),
            .added_nodes = added_nodes,
            .removed_nodes = removed_nodes,
            .previous_sibling = previous_sibling,
            .next_sibling = next_sibling,
            .attribute_name = name,
            .attribute_namespace = namespace_,
            .old_value = mapped_old_value
        });
    }

    mutation_observers::queue_mutation_observers_microtask();
}


void
dom::helpers::mutation_observers::queue_tree_mutation_record(
        nodes::event_target* target,
        ext::vector<nodes::node*> added_nodes,
        ext::vector<nodes::node*> removed_nodes,
        nodes::node* previous_sibling,
        nodes::node* next_sibling) {

    queue_mutation_record("childList", target, "", "", "", added_nodes, removed_nodes, previous_sibling, next_sibling);
}


void dom::helpers::mutation_observers::queue_mutation_observers_microtask() {
    queue_microtask(&notify_mutation_observers);
}


void
dom::helpers::mutation_observers::queue_media_element_task(
        html::elements::html_media_element* element,
        steps_t&& steps) {

    queue_element_task(element->element_event_task_source, element, steps);
}
