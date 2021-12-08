#include "custom_elements.hpp"

#include <cassert>

#include <dom/helpers/exceptions.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/element.hpp>


dom::nodes::element*
dom::helpers::custom_elements::create_an_element(
        nodes::document* document,
        ext::cstring& local_name,
        ext::cstring& namespace_,
        ext::cstring& prefix,
        ext::cstring& is,
        bool synchronous_custom_elements_flag) {

    nodes::element* result;
    auto* definition = lookup_custom_element_definition(document, namespace_, local_name, is);

    result->namespace_uri = namespaces::html;
    result->prefix = prefix;
    result->local_name = local_name;
    result->owner_document = document;
    result->m_custom_element_definition = nullptr;
    result->m_is = "";

    if (definition and definition->name != definition->local_name) {
        result = element_interface(local_name, namespaces::html);
        result->m_custom_element_state = "undefined";
        result->m_is = is;

        if (synchronous_custom_elements_flag) {
            v8::TryCatch exception_handler{v8::Isolate::GetCurrent()};
            upgrade_element(definition, result);

            if (exception_handler.HasCaught()) {
                console::reporting::report_warning_to_console(exception_handler.Message()->Get());
                result->m_custom_element_state = "failed";
            }
        }
        else
            enqueue_custom_element_upgrade_reaction(result, definition);
    }

    else if (definition) {
        if (synchronous_custom_elements_flag) {
            v8::TryCatch exception_handler{v8::Isolate::GetCurrent()};

            result = definition->constructor;
            assert(result->m_custom_element_state and result->m_custom_element_definition);
            assert(result->namespace_uri == namespaces::html);

            exceptions::throw_v8_exception(
                    "custom element must have an empty attribute list",
                    NOT_SUPPORTED_ERR,
                    [result] -> bool {return result->attributes->empty();});

            exceptions::throw_v8_exception(
                    "custom element must have an empty child nodes list",
                    NOT_SUPPORTED_ERR,
                    [result] -> bool {return result->child_nodes->empty();});

            exceptions::throw_v8_exception(
                    "custom element must have a null / undefined parent",
                    NOT_SUPPORTED_ERR,
                    [result] -> bool {return result->parent_node;});

            exceptions::throw_v8_exception(
                    "custom element must have a null / undefined document",
                    NOT_SUPPORTED_ERR,
                    [result] -> bool {return result->owner_document;});

            exceptions::throw_v8_exception(
                    "custom element's local name must match local name",
                    NOT_SUPPORTED_ERR,
                    [result, local_name] -> bool {return result->local_name != local_name;});

            if (exception_handler.HasCaught()) {
                console::reporting::report_warning_to_console(exception_handler.Message()->Get());

                result = new nodes::element{};
                result->m_custom_element_state = "failed";
            }
        }

        else {
            result = new nodes::element{};
            result->m_custom_element_state = "undefined";
        }
    }

    else {
        result = element_interface(local_name, namespace_);
        result->namespace_uri = namespace_;
        result->m_custom_element_state = "uncustomized";
        result->m_is = is;

        if (namespace_ == namespaces::html and is_valid_custom_element_name(local_name) or is)
            result->m_custom_element_state = "undefined";
    }

    return result;
}


void
dom::helpers::custom_elements::upgrade_element(
        internal::custom_element_definition* definition,
        nodes::element* element) {

    if (element->m_custom_element_state != "undefined" and element->m_custom_element_state != "uncustomized")
        return;

    element->m_custom_element_definition = definition;
    element->m_custom_element_state = "failed";

    for (auto* attribute: *element->attributes)
        enqueue_custom_element_callback_reaction(element, "attributeChangedCallback", ext::vector<ext::string>{attribute->local_name, "", attribute->value, attribute->namespace_uri});

    if (shadows::is_connected(element))
        enqueue_custom_element_callback_reaction(element, "connectedCallback", {});

    definition->construction_stack.append(element);
    using c = decltype(definition->constructor);

    v8::TryCatch exception_handler{v8::Isolate::GetCurrent()};
    exceptions::throw_v8_exception(
            "if the definition has shadows disabled, the element cannot have a shadow root",
            NOT_SUPPORTED_ERR,
            [definition, element] -> bool {definition->disable_shadow and element->shadow_root_node;});

    element->m_custom_element_state = "precustomized";
    auto* construction_result = new c{};

    // TODO : javascript::helpers::same_value(...)
    definition->construction_stack.pop();

    if (exception_handler.HasCaught()) {
        element->m_custom_element_definition = nullptr;
        element->m_custom_element_reaction_queue = {};
        exception_handler.ReThrow();
    }

    // TODO : form association

    element->m_custom_element_state = "custom";
}


void
dom::helpers::custom_elements::try_to_upgrade_element(
        nodes::element* element) {

    if (auto* definition = lookup_custom_element_definition(
            element->owner_document,
            element->namespace_uri,
            element->local_name,
            element->m_is))
        enqueue_custom_element_upgrade_reaction(element, definition);
}


dom::internal::custom_element_definition*
dom::helpers::custom_elements::lookup_custom_element_definition(
        nodes::document* document,
        ext::cstring& local_name,
        ext::cstring& namespace_,
        ext::cstring& is) {

    if (namespace_ != namespaces::html)
        return nullptr;

    if (not document->m_browsing_context)
        return nullptr;

    html::custom_elements::custom_element_registry registry {};
    auto* definition = registry.get(local_name);

    if (definition
            and definition->local_name == local_name
            and (definition->name == local_name or definition->name == is))
        return definition;

    return nullptr;
}


void
dom::helpers::custom_elements::enqueue_element_on_appropriate_element_queue(
        nodes::element* element) {

    // TODO
}


void
dom::helpers::custom_elements::enqueue_custom_element_callback_reaction(
        nodes::element* element,
        ext::cstring& callback_name,
        ext::vector<ext::string>&& args) {

    auto* definition = element->m_custom_element_definition;
    auto callback = definition->lifecycle_callbacks.at("callback_name");

    if (not callback)
        return;

    if (callback_name == "attributeChangedCallback") {
        ext::string attribute_name = args.front();
        if (not definition->observed_attributes.contains(attribute_name)) return;
    }
}


void
dom::helpers::custom_elements::enqueue_custom_element_upgrade_reaction(
        nodes::element* element,
        internal::custom_element_definition* definition) {

    // TODO
}


void
dom::helpers::custom_elements::enqueue_custom_element_reaction(
        std::queue<nodes::element*>& element_queue) {

    // TODO
}


bool
dom::helpers::custom_elements::is_valid_custom_element_name(
        ext::cstring& element_name) {

    // TODO
    return true;
}


bool
dom::helpers::custom_elements::is_custom_node(
        nodes::element* element) {

    return element->m_custom_element_state == "custom";
}


dom::nodes::element*
dom::helpers::custom_elements::element_interface(
        ext::cstring& local_name,
        ext::cstring& namespace_) {

    return new nodes::element{}; // TODO -> different types
}
