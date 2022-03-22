#include "mutation_observer.hpp"

#include <ext/set.hpp>
#include <javascript/environment/realms.hpp>

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_observers.hpp>

#include <dom/nodes/node.hpp>

dom::mutations::mutation_observer::mutation_observer() : dom_object();


/*
 * https://dom.spec.whatwg.org/#dom-mutationobserver-mutationobserver
 * https://developer.mozilla.org/en-US/docs/Web/API/MutationObserver/MutationObserver
 *
 * The DOM MutationObserver() constructor — part of the MutationObserver interface — creates and returns a new observer
 * which invokes a specified callback when DOM events occur.
 *
 * DOM observation does not begin immediately; the observe() method must be called first to establish which portion of
 * the DOM to watch and what kinds of changes to watch for.
 */
dom::mutations::mutation_observer::mutation_observer(
        mutation_callback&& callback)

        : dom_object()
        , m_callback(std::move(callback))
{
    // set the attributes
    m_node_list = new ext::vector<nodes::node*>{};
    m_record_queue = new std::queue<mutation_record*>{};

    javascript::realms::relevant_agent().get<ext::set<mutation_observer*>&>("mutationRecords").add(this);
}


/*
 * https://dom.spec.whatwg.org/#dom-mutationobserver-observe
 * https://developer.mozilla.org/en-US/docs/Web/API/MutationObserver/observe
 *
 * The MutationObserver method observe() configures the MutationObserver callback to begin receiving notifications of
 * changes to the DOM that match the given options.
 *
 * Depending on the configuration, the observer may watch a single Node in the DOM tree, or that node and some or all of
 * its descendant nodes.
 *
 * To stop the MutationObserver (so that none of its callbacks will be triggered any longer), call
 * MutationObserver.disconnect().
 */
auto dom::mutations::mutation_observer::observe(
        const nodes::node* const target,
        ext::string_any_map&& options)
        -> void
{
    // extract the attribute filter as a vector of strings
    auto attribute_filter = options.at("attribute_filter").to<ext::vector<ext::string>>();

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
    helpers::exceptions::throw_v8_exception(
            "if 'childList' is true, one from ['attributes', 'characterData'] must also be true",
            &v8::Exception::TypeError,
            [options] {return not options.at("childList").to<bool>() or options.at("attributes").to<bool>() or options.at("characterData").to<bool>();});

    // if the <attributeOldValue> option is true but the <attribute> value is false, then trow an error
    helpers::exceptions::throw_v8_exception(
            "if 'attributesOldValue' is true, 'attributes' must be true",
            &v8::Exception::TypeError,
            [options] {return options.at("attributesOldValue").to<bool>() and not options.at("attributes").to<bool>();});

    // if the <attributeFilter> option isn't empty and the <attribute> option is false, then throw as error
    helpers::exceptions::throw_v8_exception(
            "if 'attributesFilter' is not empty, 'attributes' must be true",
            &v8::Exception::TypeError,
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


auto dom::mutations::mutation_observer::disconnect() -> void
{
    // iterate over each node that the observer is listening to and remove them if their observer is this object
    for (auto* node: *m_node_list)
        node->m_registered_observer_list->remove_if([this](internal::registered_observer* internal_registered) {return internal_registered->observer == this;});

    // empty the record queue because this observer is now disconnected
    delete m_record_queue;
    m_record_queue = nullptr;
}


/*
 * https://dom.spec.whatwg.org/#dom-mutationobserver-takerecords
 * https://developer.mozilla.org/en-US/docs/Web/API/MutationObserver/takeRecords
 *
 * The MutationObserver method takeRecords() returns a list of all matching DOM changes that have been detected but not
 * yet processed by the observer's callback function, leaving the mutation queue empty.
 *
 * The most common use case for this is to immediately fetch all pending mutation records immediately prior to
 * disconnecting the observer, so that any pending mutations can be processed when shutting down the observer.
 */
auto dom::mutations::mutation_observer::take_records() -> ext::vector<dom::mutations::mutation_record*>
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


/*
 * https://dom.spec.whatwg.org/#dom-mutationobserver-disconnect
 * https://developer.mozilla.org/en-US/docs/Web/API/MutationObserver/disconnect
 *
 * The MutationObserver method disconnect() tells the observer to stop watching for mutations.
 *
 * The observer can be reused by calling its observe() method again.
 */
auto dom::mutations::mutation_observer::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<mutation_record>{isolate}
            .template ctor<mutation_observer::mutation_callback&&>()
            .template function("observe", &mutation_observer::observe)
            .template function("disconnect", &mutation_observer::disconnect)
            .template function("takeRecords", &mutation_observer::take_records)
            .auto_wrap_objects();
}
