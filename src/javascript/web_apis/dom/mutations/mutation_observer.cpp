#include "mutation_observer.hpp"

#include <ext/set.hpp>
#include <javascript/environment/realms.hpp>

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_observers.hpp>

#include <dom/nodes/node.hpp>

dom::mutations::mutation_observer::mutation_observer() : dom_object();


dom::mutations::mutation_observer::mutation_observer(
        mutation_callback&& callback)

        : dom_object()
        , m_callback(std::move(callback))
{
    // set the attribute values
    m_node_list = new ext::vector<nodes::node*>{};
    m_record_queue = new std::queue<mutation_record*>{};

    javascript::realms::relevant_agent().get<ext::set<mutation_observer*>&>("mutationRecords").add(this);
}


auto dom::mutations::mutation_observer::observe(
        const nodes::node* const target,
        ext::string_any_map&& options)
        -> void
{
    // extract the attribute filter as a vector of strings
    auto attribute_filter = options.at("attribute_filter").to<ext::string_vector>();

    // if the <attributeFilter> option isn't empty, then set the <attribute> option to true
    if (not attribute_filter.empty())
        options.at("attributes") = true;

    // if the <attributes> option is false but the <attributeOldValue> option is true, then set the <attribute> option
    // to true
    if (not options.at("attributes").to<bool>() and options.at("attributeOldValue").to<bool>())
        options.at("attributes") = true;

    // if the <characterData> option is false but the <characterOldValue> option is true, then set the <character>
    // option to true
    if (not options.at("characterData").to<bool>() and options.at("characterDataOldValue").to<bool>())
        options.at("characterData") = true;

    // if the <attributes> or <characterData> option is true but the <childList> option is false, then throw an error
    helpers::exceptions::throw_v8_exception<V8_TYPE_ERROR>(
            "if 'childList' is true, one from ['attributes', 'characterData'] must also be true",
            [options] {return not options.at("childList").to<bool>() or options.at("attributes").to<bool>() or options.at("characterData").to<bool>();});

    // if the <attributeOldValue> option is true but the <attribute> value is false, then trow an error
    helpers::exceptions::throw_v8_exception<V8_TYPE_ERROR>(
            "if 'attributesOldValue' is true, 'attributes' must be true",
            [options] {return options.at("attributesOldValue").to<bool>() and not options.at("attributes").to<bool>();});

    // if the <attributeFilter> option isn't empty and the <attribute> option is false, then throw as error
    helpers::exceptions::throw_v8_exception<V8_TYPE_ERROR>(
            "if 'attributesFilter' is not empty, 'attributes' must be true",
            [options, &attribute_filter] {return not attribute_filter.empty() and not options.at("attributes").to<bool>();});

    // iterate over the registered observer list
    for (const auto* registered: *target->m_registered_observer_list)
    {
        // only apply if the registered observer's observer is this mutation_observer
        if (registered->observer == this)
        {
            // iterate over each node that the observer is listening to
            for (auto* node: *m_node_list)
            {
                // remove all the registered_observers whose source is
                node->m_registered_observer_list
                        ->cast_all<internal::transient_registered_observer*>()
                        .remove_if([registered](internal::transient_registered_observer* internal_registered) {return internal_registered->source == registered;}, true);
            }

            // set the registered options to the options passed in
            registered->options = options;
        }

        // if the registered observer's observer is not this mutation_observer
        else
        {
            target->m_registered_observer_list->append(new internal::registered_observer{.observer=this, .options=options});
            m_node_list->append(target);
        }
    }
}


auto dom::mutations::mutation_observer::disconnect()
        -> void
{
    // iterate over each node that the observer is listening to and remove them if their observer is this object
    for (auto* node: *m_node_list)
        node->m_registered_observer_list->remove_if([this](internal::registered_observer* internal_registered) {return internal_registered->observer == this;});

    // empty the record queue because this observer is now disconnected
    delete m_record_queue;
    m_record_queue = nullptr;
}


auto dom::mutations::mutation_observer::take_records()
        -> ext::vector<dom::mutations::mutation_record*>
{
    // copy the queue into a vector
    ext::vector<mutation_record*> vector {};
    vector = *m_record_queue;

    // empty the record queue because this observer is now disconnected
    delete m_record_queue;
    m_record_queue = nullptr;

    // return the vector
    return vector;

}


auto dom::mutations::mutation_observer::v8(
        v8::Isolate* isolate)
        const -> ext::any
{
    return v8pp::class_<mutation_record>{isolate}
            .ctor<mutation_observer::mutation_callback&&>()
            .inherit<dom_object>()
            .function("observe", &mutation_observer::observe)
            .function("disconnect", &mutation_observer::disconnect)
            .function("takeRecords", &mutation_observer::take_records)
            .auto_wrap_objects();
}
