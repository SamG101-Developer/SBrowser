#include "mutation_observer.hpp"

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_observers.hpp>

#include <dom/nodes/node.hpp>


dom::mutations::mutation_observer::mutation_observer(
        mutation_callback&& callback) {

    m_callback = callback;
    m_record_queue = new std::queue<mutation_record*>{};
}


void
dom::mutations::mutation_observer::observe(
        nodes::node* target,
        ext::string_any_map&& options) {

    auto attribute_filter = (ext::vector<ext::string>)(options.at("attribute_filter"));

    if (not options.at("attributes") and (options.at("attributeOldValue") or not attribute_filter.empty()))
        options.at("attributes") = true;

    if (not options.at("characterData") and options.at("characterDataOldValue"))
        options.at("characterData") = true;

    helpers::exceptions::throw_v8_exception(
            "one item from ['childList', 'attributes', 'characterData'] must be true",
            &v8::Exception::TypeError,
            [options] {return not (options.at("childList") or options.at("attributes") or options.at("characterData"));});

    helpers::exceptions::throw_v8_exception(
            "if 'attributesOldValue' is true, 'attributes' must be true",
            &v8::Exception::TypeError,
            [options] {return options.at("attributesOldValue") and not options.at("attributes");});

    helpers::exceptions::throw_v8_exception(
            "if 'attributesFilter' is not empty, 'attributes' must be true",
            &v8::Exception::TypeError,
            [options, attribute_filter] {return not attribute_filter.empty() and not options.at("attributes");});

    for (const auto* registered: *target->m_registered_observer_list) {
        if (registered->observer == this) {
            for (auto* node: *m_node_list)
                node->m_registered_observer_list->remove_if([registered](auto* internal_registered) {return internal_registered->source == registered;});

            registered->options = options;
        }
        else {
            target->m_registered_observer_list->append(new internal::registered_observer{.observer=this, .options=options});
            m_node_list->append(target);
        }
    }
}


void
dom::mutations::mutation_observer::disconnect() {
    for (auto* node: *m_node_list)
        node->m_registered_observer_list->remove_if([this](auto* internal_registered) {return internal_registered->observer == this;});
    std::queue<mutation_record*>{}.swap(*m_record_queue);
}


ext::vector<dom::mutations::mutation_record*>
dom::mutations::mutation_observer::take_records() {
    ext::vector<mutation_record*> vector {};
    for (size_t index = 0; index < m_record_queue->size(); ++index) {
        vector.append(m_record_queue->front());
        m_record_queue->pop();
    }

    return vector;

}
