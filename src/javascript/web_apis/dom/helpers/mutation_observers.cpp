#include "mutation_observers.hpp"

#include <unordered_set>
#include <queue>

#include <ext/map.hpp>

#include <javascript/environment/realms.hpp>

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/mutations/mutation_observer.hpp>
#include <dom/mutations/mutation_record.hpp>

#include <dom/nodes/node.hpp>

#include <html/elements/html_slot_element.hpp>

#include <v8.h>


void dom::helpers::mutation_observers::notify_mutation_observers() {

    javascript::realms::surrounding_agent().set("mutation_observer_microtask_queue", false);
    auto notify_set = javascript::realms::surrounding_agent().get<std::unordered_set<mutations::mutation_observer*>>("mutation_observers");
    auto signal_slots_set = javascript::realms::surrounding_agent().get<std::unordered_set<html::elements::html_slot_element*>>("signal_slots");
    javascript::realms::surrounding_agent().get<decltype(signal_slots_set)>("signal_slots").clear();

    for (auto* mutation_observer: notify_set) {
        auto* records = new ext::vector<mutations::mutation_record*>{};
        for (size_t index = 0; index < mutation_observer->m_record_queue->size(); ++index) {
            records->append(mutation_observer->m_record_queue->front());
            mutation_observer->m_record_queue->pop();
        }

        for (auto* node: *mutation_observer->m_node_list)
            node->m_registered_observer_list->remove_if([mutation_observer](auto* observer) {return observer->observer == mutation_observer;});

        if (not records->empty()) {
            v8::TryCatch exception_handler{v8::Isolate::GetCurrent()};
            mutation_observer->m_callback(records, mutation_observer);

            if (exception_handler.HasCaught()) ; // TODO : console::reporting::report_warning_to_console(exception_handler.Message()->Get());
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
        ext::cvector<nodes::node*>& added_nodes,
        ext::cvector<nodes::node*>& removed_nodes,
        nodes::node* previous_sibling,
        nodes::node* next_sibling) {

    ext::map<mutations::mutation_observer*, ext::string> interested_observers;
    ext::vector<nodes::node*> nodes = trees::ancestors((nodes::node*)target);

    for (auto* node: nodes) {
        for (auto* registered: *node->m_registered_observer_list) {
            auto options = registered->options;
            auto attribute_filter = (ext::vector<ext::string>)options.at("attributeFilter");

            if (not ((node != target and not options.at("subtree")))
                    or (type == "attributes" and not options.at("attributes"))
                    or (type == "attributes" and options.at("attributeFilter") and (not attribute_filter.contains(name) or namespace_))
                    or (type == "characterData" and not options.at("characterData"))
                    or (type == "childList" and not options.at("childList"))) {

                mutations::mutation_observer* mutation_observer = registered->observer;

                if (not interested_observers.has_key(mutation_observer))
                    interested_observers.at(mutation_observer) = "";

                if ((type == "attributes" and options.at("attributeOldValue") or type == "characterData" and options.at("characterDataOldValue")))
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
        mutation_record->target = dynamic_cast<nodes::node*>(target);
        mutation_record->previous_sibling = previous_sibling;
        mutation_record->next_sibling = next_sibling;
        mutation_record->added_nodes = added_nodes;
        mutation_record->removed_nodes = removed_nodes;

        observer->m_record_queue->push(mutation_record);
    }

    mutation_observers::queue_mutation_observers_microtask();
}


void
dom::helpers::mutation_observers::queue_tree_mutation_record(
        nodes::event_target* target,
        ext::cvector<nodes::node*>& added_nodes,
        ext::cvector<nodes::node*>& removed_nodes,
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
