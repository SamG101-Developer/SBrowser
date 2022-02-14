#include "attributes.hpp"

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/node_internals.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/element.hpp>


void
dom::helpers::attributes::handle_attributes_changes(
        const nodes::attr* attribute,
        const nodes::element* owner_element,
        ext::cstring& old_value,
        ext::cstring& new_value)
{
    // queue a mutation record describing the change in the attribute
    mutation_observers::queue_mutation_record("attributes", owner_element, attribute->local_name, attribute->namespace_uri, old_value, {}, {}, nullptr, nullptr);

    // if the element is custom, queue a custom element reaction
    if (owner_element->m_custom_element_state == "custom")
        custom_elements::enqueue_custom_element_callback_reaction(owner_element, "attributeCallbackChanged", ext::vector<ext::string>{attribute->local_name, old_value, new_value, attribute->namespace_uri});
}


void
dom::helpers::attributes::change(
        nodes::attr* attribute,
        ext::cstring& new_value)
{
    // handle the attribute changes
    handle_attributes_changes(attribute, attribute->owner_element, attribute->value, new_value);

    // set the attribute value to the new_value
    attribute->value = new_value;
}


void
dom::helpers::attributes::append(
        nodes::attr* attribute,
        nodes::element* new_owner_element)
{
    // handle the attribute changes
    handle_attributes_changes(attribute, attribute->owner_element, "", attribute->value);

    // set the attribute's owner element to new_owner_element, and append the attribute to the new_owner_element's
    // attribute list
    new_owner_element->attributes->append(attribute);
    attribute->owner_element = new_owner_element;
}


void
dom::helpers::attributes::remove(
        nodes::attr* attribute)
{
    // handle the attribute changes
    handle_attributes_changes(attribute, attribute->owner_element, attribute->value, "");

    // remove the attribute from the element's attribute list, and clear the owner element pointer
    attribute->owner_element->attributes->remove(attribute);
    attribute->owner_element = nullptr;

    // delete the pointer to the attribute
    delete attribute;
}


void
dom::helpers::attributes::replace(
        nodes::attr* old_attribute,
        nodes::attr* new_attribute)
{
    // handle the attribute changes
    handle_attributes_changes(old_attribute, old_attribute->owner_element, old_attribute->value, new_attribute->value);

    // replace the attribute in the owner element's list, set the owner_element to the new attribute, and clear the
    // owner_element from the old_attribute
    old_attribute->owner_element->attributes->replace(old_attribute, new_attribute);
    new_attribute->owner_element = old_attribute->owner_element;
    old_attribute->owner_element = nullptr;

    // delete the old_attribute
    delete old_attribute;
}


ext::string
dom::helpers::attributes::get_attribute_value(
        const nodes::element* owner_element,
        ext::cstring& namespace_,
        ext::cstring& local_name)
{
    // return the first attribute's value in the element's attribute list whose namespace and local name matches
    // namespace_ and local_name
    return get_attribute_by_ns(namespace_, local_name, owner_element)->value;
}


dom::nodes::attr*
dom::helpers::attributes::get_attribute_by_name(
        ext::cstring& qualified_name,
        const nodes::element* owner_element)
{
    // if the document type is html then set the html qualified name to lowercase
    ext::cstring html_qualified_name = node_internals::is_html(owner_element)
            ? qualified_name.new_lowercase()
            : qualified_name;

    // return the first attribute in the element's attribute list whose name matched the html_qualified_name
    return owner_element->attributes->first_match([&html_qualified_name](const nodes::attr* attribute) {return attribute->name == html_qualified_name;});
}


dom::nodes::attr*
dom::helpers::attributes::get_attribute_by_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name,
        const nodes::element* owner_element)
{
    // return the first attribute in the element's attribute list whose namespace and local name matches namespace_ and
    // local_name
    return owner_element->attributes->first_match([&local_name, &namespace_](const nodes::attr* attribute) {return attribute->namespace_uri == namespace_ and attribute->local_name == local_name;});
}


dom::nodes::attr*
dom::helpers::attributes::set_attribute_value(
        nodes::element* owner_element,
        ext::cstring& local_name,
        ext::cstring& value,
        ext::cstring& prefix,
        ext::cstring& namespace_)
{
    // get the attribute by matching the namespace, local name and owner_element
    auto* attribute = get_attribute_by_ns(namespace_, local_name, owner_element);

    // if the attribute doesn't exist, create it and set its attributes from the parameters
    if (not attribute) {
        attribute = new nodes::attr{};
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


dom::nodes::attr*
dom::helpers::attributes::set_attribute_by_name(
        ext::cstring& qualified_name,
        const nodes::element* owner_element,
        ext::cstring& value)
{
    // if the document type is html then set the html qualified name to lowercase
    ext::string html_qualified_name = helpers::node_internals::is_html(owner_element)
            ? qualified_name.new_lowercase()
            : qualified_name;

    // get the attribute by matching the name
    nodes::attr* attribute = get_attribute_by_name(qualified_name, owner_element); // owner_element->attributes->first_match([qualified_name](nodes::attr* attribute) {return attribute->name == qualified_name;});

    // if the attribute doesn't exist, create it and set its attributes from the parameters
    if (not attribute) {
        attribute = new nodes::attr{};
        attribute->local_name = qualified_name;
        attribute->value = value;
        attribute->owner_document = owner_element->owner_document;
        helpers::attributes::append(attribute, owner_element);
    }

    // change the value to value, so that the mutations are updated
    helpers::attributes::change(attribute, value);
}


dom::nodes::attr*
dom::helpers::attributes::set_attribute_by_ns(
        ext::cstring& namespace_,
        ext::cstring& qualified_name,
        const nodes::element* owner_element,
        ext::cstring& value)
{
    // extract the namespace, prefix and local name from the namespace and qualified name
    auto [html_qualified_namespace, prefix, local_name] = namespaces::validate_and_extract(namespace_, qualified_name);

    // if the document type is html then set the html qualified name to lowercase
    ext::string html_qualified_name = helpers::node_internals::is_html(owner_element)
            ? qualified_name.new_lowercase()
            : qualified_name;

    // get the attribute by matching the namespace, local name and owner element
    nodes::attr* attribute = helpers::attributes::get_attribute_by_ns(namespace_, local_name, owner_element);

    // if the attribute doesn't exist, create it and set its attributes from the parameters
    if (not attribute) {
        attribute = new nodes::attr{};
        attribute->namespace_uri = html_qualified_namespace;
        attribute->prefix = prefix;
        attribute->local_name = local_name;
        attribute->value = value;
        attribute->owner_document = owner_element->owner_document;
        helpers::attributes::append(attribute, owner_element);
    }

    // change the value to value, so that the mutations are updated
    helpers::attributes::change(attribute, value);
}


dom::nodes::attr*
dom::helpers::attributes::set_attribute(
        nodes::attr* attribute,
        nodes::element* new_owner_element) {

    // if the owner element non-nullptr and doesn't match the new owner element, then throw an inuse attribute error
    exceptions::throw_v8_exception(
            "attribute's element is non-null doesn't equal element",
            INUSE_ATTRIBUTE_ERR,
            [attribute, new_owner_element] {return attribute->owner_element and attribute->owner_element != new_owner_element;});

    // get the attribute by matching the namespace, local name and owner element
    auto* old_attribute = get_attribute_by_ns(attribute->namespace_uri, attribute->local_name, new_owner_element);

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


dom::nodes::attr*
dom::helpers::attributes::remove_attribute_by_name(
        ext::cstring& qualified_name,
        const nodes::element* owner_element)
{
    // get the attribute by matching the name and owner element
    auto* attribute = get_attribute_by_name(qualified_name, owner_element);

    // remove the attribute
    remove_attribute(attribute);

    // return the attribute
    return attribute;
}


dom::nodes::attr*
dom::helpers::attributes::remove_attribute_by_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name,
        const nodes::element* owner_element)
{
    // get the attribute by matching the namespace, local name and owner element
    auto* attribute = get_attribute_by_ns(namespace_, local_name, owner_element);

    // remove the attribute
    remove_attribute(attribute);

    // return the attribute
    return attribute;
}


dom::nodes::attr*
dom::helpers::attributes::remove_attribute(
        nodes::attr* attribute,
        nodes::element* owner_element)
{
    // if the attribute isn't in the element's attribute list, then return early
    if (not owner_element->attributes->contains(attribute))
        return attribute;

    // change the value to empty, remove the attribute, and remove it from the element's attribute list
    change(attribute, "");
    remove(attribute);
    owner_element->attributes->remove(attribute);

    // return the attribute
    return attribute;
}


dom::nodes::attr*
dom::helpers::attributes::toggle_attribute_by_name(
        ext::cstring& qualified_name,
        const nodes::element* owner_element,
        bool force)
{
    // get the attribute by matching the name and owner element
    auto* attribute = get_attribute_by_name(qualified_name, owner_element);

    // toggle the attribute and return it
    return toggle_attribute(attribute, force);

}


dom::nodes::attr*
dom::helpers::attributes::toggle_attribute_by_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name,
        const nodes::element* owner_element,
        bool force) {

    // get the attribute by matching the namespace, local name and owner element
    auto* attribute = get_attribute_by_ns(namespace_, local_name, owner_element);

    // toggle the attribute and return uit
    return toggle_attribute(attribute, force);
}


dom::nodes::attr*
dom::helpers::attributes::toggle_attribute(
        nodes::attr* attribute,
        bool force) // TODO : tidy this method
{
    if (not attribute)
    {
        if (force)
        {
            attribute = new nodes::attr{};
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
        return false;
    }

    return true
}


void
dom::helpers::attributes::set_existing_attribute_value(
        nodes::attr* attribute,
        ext::cstring& value)
{
    // if attribute doesn't have an owner element, then set the value to value
    if (not attribute->owner_element)
        attribute->value = value;

    // otherwise, change the attributes value to value
    else
        change(attribute, value);
}
