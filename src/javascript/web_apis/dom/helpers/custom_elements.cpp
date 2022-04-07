#include "custom_elements.hpp"

#include <cassert>

#include <ext/string.hpp>

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/shadows.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/element.hpp>

#include <html/elements/html_unknown_element.hpp>
#include <html/helpers/custom_html_elements.hpp>


template <typename T>
auto dom::helpers::custom_elements::create_an_element(
        nodes::document* const document,
        const ext::string& local_name,
        const ext::string& namespace_,
        const ext::string& prefix,
        const ext::string& is,
        const bool synchronous_custom_elements_flag)
        -> T* {

    // create a result pointer to store the created element, and a definition struct holding private information about
    // the custom element (like an PIMPL without the pointer -> uses a lookup table instead)
    nodes::element* result;
    auto* const definition = lookup_custom_element_definition(document, local_name, namespace_, is);

    // if there is a valid definition for the custom element, and the definition's name doesn't equal the definition's
    // local name, then...
    if (definition and definition->name != definition->local_name)
    {
        // create an element from the local name, in the html namespace, and an undefined custom element state - the
        // custom element definition is null, because the customization is undefined
        result = html::helpers::custom_html_elements::element_interface(local_name, namespaces::HTML);
        result->namespace_uri = namespaces::HTML;
        result->prefix = prefix;
        result->local_name = local_name;
        result->owner_document = document;
        result->m_custom_element_state = "undefined";
        result->m_custom_element_definition = nullptr;
        result->m_is = is;

        // if the synchronous_custom_elements_flag is set, then introduce a javascript try-except block, and upgrade
        // the element
        if (synchronous_custom_elements_flag) {

            // try to upgrade the element, catching any javascript exceptions in the process
            const v8::TryCatch exception_handler{v8::Isolate::GetCurrent()};
            upgrade_element(definition, result);

            // if there was an exception thrown, then the custom element state is changed to "failed"
            if (exception_handler.HasCaught())
            {
                // TODO console::reporting::report_warning_to_console(exception_handler.Message()->Get());
                result->m_custom_element_state = "failed";
            }
        }
        else
        {
            // otherwise, queue a custom element upgrade, as the upgrade was successful, and the mutation observers or
            // similar need to be notified
            enqueue_custom_element_upgrade_reaction(result, definition);
        }
    }

    // otherwise, if there is a definition
    else if (definition)
    {
        // if the synchronous_custom_elements_flag is set, then run all the exception checks
        if (synchronous_custom_elements_flag)
        {
            // try to upgrade the element, catching any javascript exceptions in the process
            const v8::TryCatch exception_handler{v8::Isolate::GetCurrent()};

            // set the result to the definition's constructor
            result = definition->constructor;

            // assert that the custom element state and custom element definition are both non-nullptr, and the
            // namespace is the HTML namespace
            assert(not result->m_custom_element_state.empty() and result->m_custom_element_definition);
            assert(result->namespace_uri == namespaces::HTML);

            // if the result's attribute list is not empty, then throw a not supported error
            exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
                    "custom element must have an empty attribute list",
                    [&result] {return not result->attributes->empty();});

            // if the result's child list is not empty, then throw a not supported error
            exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
                    "custom element must have an empty child nodes list",
                    [&result] {return not result->child_nodes->empty();});

            // if the result has a parent, then throw a not supported error
            exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
                    "custom element must have a null / undefined parent",
                    [&result] {return result->parent;});

            // if the result has an owner document, then throw a not supported error
            exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
                    "custom element must have a null / undefined document",
                    [&result] {return result->owner_document;});

            // if the result's local name != local_name, then throw a not supported error
            exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
                    "custom element's local name must match local name",
                    [result, local_name] {return result->local_name != local_name;});

            // set the prefix and is value to the result
            result->prefix = prefix;
            result->m_is = "";

            // if there was an exception thrown, then set up the result as a html unknown element
            if (exception_handler.HasCaught())
            {
                // TODO console::reporting::report_warning_to_console(exception_handler.Message()->Get());

                // create a html unknown element and set the attributes
                result = std::unique_ptr<html::elements::html_unknown_element>{}.get();
                result->namespace_uri = namespaces::HTML;
                result->prefix = prefix;
                result->local_name = local_name;
                result->owner_document = document;
                result->m_custom_element_definition = nullptr;
                result->m_custom_element_state = "failed";
                result->m_is = "";
            }
        }

        // otherwise, create an element with an "undefined" custom element state
        else
        {
            // create the result and set the custom element state
            result = std::unique_ptr<nodes::element>{}.get();
            result->m_custom_element_state = "undefined";
        }
    }

    // otherwise, if there is no definition
    else
    {
        // set the result to the definition's constructor, and the custom element state to "uncustomized", as there is
        // no custom definition, but the element is still valid ie a normal html element etc
        result = html::helpers::custom_html_elements::element_interface(local_name, namespace_);
        result->namespace_uri = namespace_;
        result->m_custom_element_state = "uncustomized";
        result->m_is = is;

        // if the namespace is the html namespace and the local name is a valid custom element, or <is> is set, then
        // set the custom element state to "undefined", because the element has a valid custom name, but has no custim
        // element definition
        if (namespace_ == namespaces::HTML and is_valid_custom_element_name(local_name) or not is.empty())
            result->m_custom_element_state = "undefined";
    }

    // return the result element (anything derived from dom::nodes::element*), cast up to a dom::nodes::element*
    return dynamic_cast<T*>(result);
}



auto dom::helpers::custom_elements::upgrade_element(
        internal::custom_element_definition* const definition,
        nodes::element* const element)
        -> void
{
    // elements whose custom element state that is "undefined" or "uncustomized" cannot be upgraded, so return from the
    // function early
    if (element->m_custom_element_state != "undefined" and element->m_custom_element_state != "uncustomized")
        return;

    // set the custom element definition to definition, and the custom element state to "failed" (as a default ie if no
    // other upgrade works, them the upgrade has failed)
    element->m_custom_element_definition = definition;
    element->m_custom_element_state = ext::string{"failed"};

    // iterate over the attributes in the element's attribute list, and enqueue a custom element reaction for each
    // attribute for attributeChangedCallback
    for (auto* const attribute: *element->attributes)
        enqueue_custom_element_callback_reaction(element, "attributeChangedCallback", ext::string_vector{attribute->local_name, "", attribute->value, attribute->namespace_uri});

    // if the element is connected, then enqueue a custom element reaction for the connectedCallback function
    if (shadows::is_connected(element))
        enqueue_custom_element_callback_reaction(element, "connectedCallback", {});

    // append the element to the construction stack of the definition, and map the type of the element to <c>
    definition->construction_stack.append(element);
    using c = decltype(definition->constructor);

    // create a try-catch instance to catch any exceptions caused from the element upgrade
    v8::TryCatch exception_handler{v8::Isolate::GetCurrent()};

    // if the definition has shadows disabled, and the element has a shadow root, then throw a not supported error
    exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
            "if the definition has shadows disabled, the element cannot have a shadow root",
            [definition, element] {return definition->disable_shadow and element->shadow_root_node;});

    // set the custom element state to "precustomized", as the element is being upgraded, but hasn't been upgraded yet
    element->m_custom_element_state = "precustomized";

    // create a new instance of the upgraded element
    auto* const construction_result = new c{};

    // TODO : javascript::helpers::same_value(...)
    definition->construction_stack.pop();

    // if there was a javascript exception thrown, set the custom element definition to nullptr, empty the custom
    // element reaction queue, and rethrow the error
    if (exception_handler.HasCaught())
    {
        element->m_custom_element_definition = nullptr;
        element->m_custom_element_reaction_queue = {};
        exception_handler.ReThrow();
    }

    // TODO : form association

    // at this point, no errors can be active ie they have been rethrown, so the element has been upgraded and is
    // therefore fully customized is "custom"
    element->m_custom_element_state = "custom";
}


auto dom::helpers::custom_elements::try_to_upgrade_element(
        const nodes::element* const element)
        -> void
{
    // only queue a custom upgrade reaction if a definition is found
    if (auto* const definition = lookup_custom_element_definition(element->owner_document, element->namespace_uri, element->local_name, element->m_is))
        enqueue_custom_element_upgrade_reaction(element, definition);
}


auto dom::helpers::custom_elements::lookup_custom_element_definition(
        const nodes::document* const document,
        const ext::string& local_name,
        const ext::string& namespace_,
        const ext::string& is)
        -> dom::internal::custom_element_definition*
{
    // return if the namespace isn't html
    if (namespace_ != namespaces::HTML)
        return nullptr;

    // return if the document doesn't have a browsing context
    if (not document->m_browsing_context)
        return nullptr;

/*
 * html::other::custom_element_registry registry {};
 *    auto* definition = registry.get(local_name);
 *
 *    if (definition
 *            and definition->local_name == local_name
 *            and (definition->name == local_name or definition->name == is))
 *        return definition;
 */

    return nullptr;
}


auto dom::helpers::custom_elements::enqueue_element_on_appropriate_element_queue(
        const nodes::element* const element)
        -> void
{
    // TODO
}


auto dom::helpers::custom_elements::enqueue_custom_element_callback_reaction(
        const nodes::element* const element,
        const ext::string& callback_name,
        ext::string_vector&& args)
        -> void
{
    // get the definition from the element, and the callback from it
    const auto* const definition = element->m_custom_element_definition;

    // return if there is no callback
    if (not definition->lifecycle_callbacks.has_key("callback_name"))
        return;

    // if the callback name is attributeChangedCallback
    if (callback_name == "attributeChangedCallback")
    {
        // get the attribute name and return if there is no observed attributes on the definition containing the
        // attribute name
        const ext::string attribute_name = args.front();
        if (not definition->observed_attributes.contains(attribute_name))
            return;
    }
}


auto dom::helpers::custom_elements::enqueue_custom_element_upgrade_reaction(
        const nodes::element* const element,
        const internal::custom_element_definition* const definition)
        -> void
{
    // TODO
}


auto dom::helpers::custom_elements::enqueue_custom_element_reaction(
        const std::queue<nodes::element*>& element_queue)
        -> void
{
    // TODO
}


auto dom::helpers::custom_elements::is_valid_custom_element_name(
        const ext::string& element_name)
        -> bool
{
    // TODO
    return true;
}


auto dom::helpers::custom_elements::is_custom_node(
        const nodes::element* const element)
        -> bool
{
    return element->m_custom_element_state == "custom";
}


