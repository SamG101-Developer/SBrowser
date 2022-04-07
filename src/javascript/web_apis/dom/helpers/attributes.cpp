#include "attributes.hpp"

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/node_internals.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/element.hpp>

#include <veque.hpp>


auto dom::helpers::attributes::handle_attributes_changes(
        const nodes::attr* const attribute,
        const nodes::element* const owner_element,
        const ext::string& old_value,
        const ext::string& new_value)
        -> void
{
    // queue a mutation record describing the change in the attribute
    mutation_observers::queue_mutation_record("attributes", owner_element, attribute->local_name, attribute->namespace_uri, old_value, {}, {}, nullptr, nullptr);

    // if the element is custom, queue a custom element reaction
    if (owner_element->m_custom_element_state == "custom")
        custom_elements::enqueue_custom_element_callback_reaction(owner_element, "attributeCallbackChanged", ext::string_vector{attribute->local_name, old_value, new_value, attribute->namespace_uri});
}


auto dom::helpers::attributes::change(
        nodes::attr* const attribute,
        const ext::string& new_value)
        -> void
{
    // handle the attribute changes
    handle_attributes_changes(attribute, attribute->owner_element, attribute->value, new_value);

    // set the attribute value to the new_value
    attribute->value = new_value;
}


auto dom::helpers::attributes::append(
        nodes::attr* const attribute,
        nodes::element* const new_owner_element)
        -> void
{
    // handle the attribute changes
    handle_attributes_changes(attribute, attribute->owner_element, "", attribute->value);

    // set the attribute's owner element to new_owner_element, and append the attribute to the new_owner_element's
    // attribute list
    new_owner_element->attributes->append(attribute);
    attribute->owner_element = new_owner_element;
}


auto dom::helpers::attributes::remove(
        nodes::attr* attribute)
        -> void
{
    // handle the attribute changes
    handle_attributes_changes(attribute, attribute->owner_element, attribute->value, "");

    // remove the attribute from the element's attribute list
    attribute->owner_element->attributes->remove(attribute);

    // clear the owner element pointer
    attribute->owner_element = nullptr;
    attribute = nullptr;
}


auto dom::helpers::attributes::replace(
        nodes::attr* old_attribute,
        nodes::attr* const new_attribute)
        -> void
{
    // handle the attribute changes
    handle_attributes_changes(old_attribute, old_attribute->owner_element, old_attribute->value, new_attribute->value);

    // replace the attribute in the owner element's list
    old_attribute->owner_element->attributes->replace(old_attribute, new_attribute);

    // set the owner_element to the new attribute, and clear the owner_element from the old_attribute
    new_attribute->owner_element = old_attribute->owner_element;
    old_attribute->owner_element = nullptr;
    old_attribute = nullptr;
}


auto dom::helpers::attributes::get_attribute_value(
        const nodes::element* const owner_element,
        const ext::string& local_name,
        const ext::string& namespace_)
        -> ext::string
{
    // return the first attribute's value in the element's attribute list whose namespace and local name matches
    // namespace_ and local_name
    return get_attribute_by_ns(owner_element, local_name, namespace_)->value;
}


auto dom::helpers::attributes::get_attribute_by_name(
        const nodes::element* const owner_element,
        const ext::string& qualified_name)
        -> dom::nodes::attr*
{
    // if the document type is html then set the html qualified name to lowercase
    const ext::string& html_qualified_name = node_internals::is_html(owner_element)
            ? qualified_name.new_lowercase()
            : qualified_name;

    // return the first attribute in the element's attribute list whose name matched the html_qualified_name
    return owner_element->attributes->first_match([html_qualified_name](const nodes::attr* const attribute) {return attribute->name == html_qualified_name;});
}


auto dom::helpers::attributes::get_attribute_by_ns(
        const nodes::element* const owner_element,
        const ext::string& local_name,
        const ext::string& namespace_)
        -> dom::nodes::attr*
{
    // return the first attribute in the element's attribute list whose namespace and local name matches namespace_ and
    // local_name
    return owner_element->attributes->first_match([local_name, namespace_](const nodes::attr* const attribute) {return attribute->local_name == local_name and attribute->namespace_uri == namespace_;});
}


auto dom::helpers::attributes::set_attribute_by_name(
        nodes::element* const owner_element,
        const ext::string& qualified_name,
        const ext::string& value)
        -> dom::nodes::attr*
{
    // if the document type is html then set the html qualified name to lowercase
    const ext::string& html_qualified_name = helpers::node_internals::is_html(owner_element)
            ? qualified_name.new_lowercase()
            : qualified_name;

    // get the attribute by matching the name
    nodes::attr* attribute = get_attribute_by_name(owner_element, qualified_name);

    // if the attribute doesn't exist, create it and set its attributes from the parameters
    if (not attribute)
    {
        attribute = std::unique_ptr<nodes::attr>{}.get();
        attribute->local_name = qualified_name;
        attribute->value = value;
        attribute->owner_document = owner_element->owner_document;
        helpers::attributes::append(attribute, owner_element);
    }

    // change the value to value, so that the mutations are updated
    helpers::attributes::change(attribute, value);

    // return the attribute
    return attribute;
}


auto dom::helpers::attributes::set_attribute_by_ns(
        nodes::element* const owner_element,
        const ext::string& qualified_name,
        const ext::string& namespace_,
        const ext::string& value)
        -> dom::nodes::attr*
{
    // extract the namespace, prefix and local name from the namespace and qualified name
    const auto [html_qualified_namespace, prefix, local_name] = namespaces::validate_and_extract(namespace_, qualified_name);

    // if the document type is html then set the html qualified name to lowercase
    const ext::string& html_qualified_name = helpers::node_internals::is_html(owner_element)
            ? qualified_name.new_uppercase()
            : qualified_name;

    // get the attribute by matching the namespace, local name and owner element
    nodes::attr* attribute = helpers::attributes::get_attribute_by_ns(owner_element, local_name, namespace_);

    // if the attribute doesn't exist, create it and set its attributes from the parameters
    if (not attribute)
    {
        attribute = std::unique_ptr<nodes::attr>{}.get();
        attribute->namespace_uri = html_qualified_namespace;
        attribute->prefix = prefix;
        attribute->local_name = local_name;
        attribute->value = value;
        attribute->owner_document = owner_element->owner_document;
        helpers::attributes::append(attribute, owner_element);
    }

    // change the value to value, so that the mutations are updated
    helpers::attributes::change(attribute, value);

    // return the attribute
    return attribute;
}


auto dom::helpers::attributes::set_attribute(
        nodes::element* const new_owner_element,
        nodes::attr* const attribute)
        -> dom::nodes::attr*
{

    // if the owner element non-nullptr and doesn't match the new owner element, then throw an inuse attribute error
    exceptions::throw_v8_exception<INUSE_ATTRIBUTE_ERR>(
            "attribute's element is non-null and doesn't equal element",
            [attribute, new_owner_element] {return attribute->owner_element and attribute->owner_element != new_owner_element;});

    // get the attribute by matching the namespace, local name and owner element
    auto* const old_attribute = get_attribute_by_ns(new_owner_element, attribute->local_name, attribute->namespace_uri);

    // if the old_attribute is the attribute being set, then return as it is already in the element
    if (old_attribute == attribute)
        return nullptr;

    // if there is an old attribute (which doesn't equal attribute), then replace it, otherwise append the attribute
    old_attribute
            ? replace(old_attribute, attribute)
            : append(attribute, new_owner_element);

    // return the old_attribute
    return old_attribute;

}


auto dom::helpers::attributes::remove_attribute_by_name(
        nodes::element* const owner_element,
        const ext::string& qualified_name)
        -> dom::nodes::attr*
{
    // get the attribute by matching the name and owner element and remove it
    auto* const attribute = get_attribute_by_name(owner_element, qualified_name);
    remove_attribute(owner_element, attribute);

    // return the attribute
    return attribute;
}


auto dom::helpers::attributes::remove_attribute_by_ns(
        nodes::element* const owner_element,
        const ext::string& local_name,
        const ext::string& namespace_)
        -> dom::nodes::attr*
{
    // get the attribute by matching the namespace, local name and owner element and remove it
    auto* const attribute = get_attribute_by_ns(owner_element, local_name, namespace_);
    remove_attribute(owner_element, attribute);

    // return the attribute
    return attribute;
}


auto dom::helpers::attributes::remove_attribute(
        nodes::element* const owner_element,
        nodes::attr* const attribute)
        -> dom::nodes::attr*
{
    // if the attribute isn't in the element's attribute list, then return early
    auto attributes = owner_element->attributes;
    if (std::find(attributes->begin(), attributes->end(), attribute) == attributes->end())
        return attribute;

    // change the value to empty, remove the attribute, and remove it from the element's attribute list
    change(attribute, "");
    remove(attribute);
    owner_element->attributes->remove(attribute);

    // return the attribute
    return attribute;
}


auto dom::helpers::attributes::toggle_attribute_by_name(
        const nodes::element* const owner_element,
        const ext::string& qualified_name,
        const bool force)
        -> dom::nodes::attr*
{
    // get the attribute by matching the name and owner element
    auto* const attribute = get_attribute_by_name(owner_element, qualified_name);

    // toggle the attribute and return it
    return toggle_attribute(attribute, force);

}


auto dom::helpers::attributes::toggle_attribute_by_ns(
        const nodes::element* const owner_element,
        const ext::string& local_name,
        const ext::string& namespace_,
        const bool force)
        -> dom::nodes::attr*
{
    // get the attribute by matching the namespace, local name and owner element
    auto* const attribute = get_attribute_by_ns(owner_element, local_name, namespace_);

    // toggle the attribute and return uit
    return toggle_attribute(attribute, force);
}


auto dom::helpers::attributes::toggle_attribute(
        nodes::attr* attribute,
        const bool force) // TODO : tidy this method
        -> dom::nodes::attr*
{
    if (not attribute)
    {
        if (force)
        {
            attribute = std::unique_ptr<nodes::attr>{}.get();
            // TODO ->local_name
            // TODO ->value
            // TODO ->node_document
            return attribute;
        }
        return nullptr;
    }

    else
    {
        // TODO remove_attribute(attribute, owner_element);
        return nullptr;
    }

    return attribute;
}


auto dom::helpers::attributes::set_attribute_value(
        nodes::element* const owner_element,
        const ext::string& local_name,
        const ext::string& value,
        const ext::string& prefix,
        const ext::string& namespace_)
        -> dom::nodes::attr*
{
    // get the attribute by matching the namespace, local name and owner_element
    auto* attribute = get_attribute_by_ns(owner_element, local_name, namespace_);

    // if the attribute doesn't exist, create it and set its attributes from the parameters
    if (not attribute)
    {
        attribute = std::unique_ptr<nodes::attr>{}.get();
        attribute->namespace_uri = namespace_;
        attribute->prefix = prefix;
        attribute->local_name = local_name;
        attribute->value = value;
        attribute->owner_document = owner_element->owner_document;
    }

    // append the attribute, and change the value to value, so that the mutations are updated
    append(attribute, owner_element);
    change(attribute, value);

    // return the attribute
    return attribute;
}


auto dom::helpers::attributes::set_existing_attribute_value(
        nodes::attr* const attribute,
        const ext::string& value)
        -> void
{
    // if attribute doesn't have an owner element, then set the value to value
    if (not attribute->owner_element)
        attribute->value = value;

    // otherwise, change the attributes value to value
    else
        change(attribute, value);
}
