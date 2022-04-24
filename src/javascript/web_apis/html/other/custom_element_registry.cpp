#include "custom_element_registry.hpp"

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/shadows.hpp>

#include <html/elements/html_unknown_element.hpp>
#include <html/helpers/custom_html_elements.hpp>


auto html::other::custom_element_registry::define(
        const ext::string& name,
        const html_element_constructor_t& constructor,
        const ext::string_any_map& options)
        -> void
{
    // TODO : ctor check

    dom::helpers::exceptions::throw_v8_exception<SYNTAX_ERR>(
            "name must be a valid custom element name",
            [name] {return dom::helpers::custom_elements::is_valid_custom_element_name(name);});

    dom::helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
            (ext::string)"element with name" + name.c_str() + "is already defined",
            [name, this] {return m_custom_element_definitions.any_of([name](dom::internal::custom_element_definition* definition) {return definition->name == name;});});

    dom::helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
            (ext::string)"element with constructor is already defined",
            [constructor, this] {return m_custom_element_definitions.any_of([constructor](dom::internal::custom_element_definition* definition) {return definition->constructor == constructor;});});

    auto local_name = name;
    auto extends = options.has_key("extends") ? options.at("extends").to<ext::string>() : "";

    if (not extends.empty())
    {
        dom::helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
                "extends parameter must not map to a valid custom element",
                [extends] {return dom::helpers::custom_elements::is_valid_custom_element_name(extends);});

        dom::helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
                "element interface is not defined in this specification",
                [extends] {return dynamic_cast<elements::html_unknown_element*>(helpers::custom_html_elements::element_interface(extends, dom::helpers::namespaces::HTML));});

        local_name = extends;
    }

    dom::helpers::exceptions::throw_v8_exception<NOT_SUPPORTED_ERR>(
            "custom element registry running flag must not be set",
            [name, this] {return m_is_element_definition_running_flag;});

    m_is_element_definition_running_flag = true;

    auto form_associated   = false;
    auto disable_internals = false;
    auto disable_shadow    = false;
    ext::string_vector observed_attributes;

    auto v8_isolate = v8::Isolate::GetCurrent();
    v8::TryCatch exception_handler{v8_isolate};
    auto v8_instance{v8pp::convert<html_element_constructor_t>::to_v8(v8_isolate, constructor)};
    auto v8_prototype{v8_instance->GetPrototype()};

    dom::helpers::exceptions::throw_v8_exception<V8_TYPE_ERROR>(
            "object's prototype must be an object",
            [v8_prototype] {return v8_prototype.As<v8::Object>().IsEmpty();});

    ext::string_vector lifecycle_callbacks{"connectedCallback", "disconnectedCallback", "adoptedCallback", "attributeChangedCallback"};
    for (const ext::string& callback_name: lifecycle_callbacks)
    {
        auto callback_value{v8_instance->Get(v8_isolate->GetCurrentContext(), v8pp::convert<ext::string>::to_v8(v8_isolate, callback_name)).ToLocalChecked()};
        auto callback_function{callback_value.As<v8::Function>()};
        // TODO
    }
}


auto html::other::custom_element_registry::get(
        const ext::string& name)
        -> std::optional<html_element_constructor_t>
{
    auto definition = m_custom_element_definitions.filter([name](dom::internal::custom_element_definition* definition) {return definition->name == name;}).front();
    return definition
            ? std::optional<html_element_constructor_t>{definition->constructor}
            : std::nullopt;
}


auto html::other::custom_element_registry::when_defined(
        const ext::string& name)
        -> webidl::types::promise<html_element_constructor_t>
{
    webidl::types::promise<html_element_constructor_t> promise;

    if (dom::helpers::custom_elements::is_valid_custom_element_name(name))
        return promise.reject(dom::other::dom_exception("name must be a valid custom element name", SYNTAX_ERR));

    if (std::optional<html_element_constructor_t> constructor = get(name); constructor.has_value())
        promise.resolve(constructor.value()());

    if (not m_when_defined_promise_map.has_key(name))
        m_when_defined_promise_map.insert(name, webidl::types::promise<html_element_constructor_t>{});

    return m_when_defined_promise_map.at(name);
}


auto html::other::custom_element_registry::upgrade(
        dom::nodes::node* root)
        -> void
{
    dom::helpers::shadows::shadow_including_descendants(root)
            .cast_all<dom::nodes::element*>()
            .for_each([](dom::nodes::node* candidate) {dom::helpers::custom_elements::try_to_upgrade_element(candidate);});
}
