#include "attributes.hpp"

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/node_internals.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/element.hpp>


void
dom::helpers::attributes::handle_attributes_changes(
        nodes::attr* attribute,
        nodes::element* owner_element,
        ext::cstring& old_value,
        ext::cstring& new_value) {

    mutation_observers::queue_mutation_record(
            "attributes", owner_element, attribute->local_name, attribute->namespace_uri, old_value, {}, {}, nullptr, nullptr);

    if (owner_element->m_custom_element_state == "custom")
        custom_elements::enqueue_custom_element_callback_reaction(
                owner_element,
                "attributeCallbackChanged",
                ext::vector<ext::string>{attribute->local_name, old_value, new_value, attribute->namespace_uri});
}


void
dom::helpers::attributes::change(
        nodes::attr* attribute,
        ext::cstring& new_value) {

    handle_attributes_changes(attribute, attribute->owner_element, attribute->value, new_value);
    attribute->value = new_value;
}


void
dom::helpers::attributes::append(
        nodes::attr* attribute,
        nodes::element* new_owner_element) {

    handle_attributes_changes(attribute, attribute->owner_element, "", attribute->value);
    new_owner_element->attributes->append(attribute);
    attribute->owner_element = new_owner_element;
}


void
dom::helpers::attributes::remove(nodes::attr* attribute) {

    handle_attributes_changes(attribute, attribute->owner_element, attribute->value, "");
    attribute->owner_element->attributes->remove(attribute);
    attribute->owner_element = nullptr;
    delete attribute;
}


void
dom::helpers::attributes::replace(
        nodes::attr* old_attribute,
        nodes::attr* new_attribute) {

    handle_attributes_changes(old_attribute, old_attribute->owner_element, old_attribute->value, new_attribute->value);
    old_attribute->owner_element->attributes->replace(old_attribute, new_attribute);
    new_attribute->owner_element = old_attribute->owner_element;
    old_attribute->owner_element = nullptr;
    delete old_attribute;
}


dom::nodes::attr*
dom::helpers::attributes::get_attribute_by_name(
        ext::cstring& qualified_name,
        nodes::element* owner_element) {

    ext::string html_qualified_name = node_internals::is_html(owner_element)
            ? qualified_name.new_lowercase()
            : qualified_name;

    return owner_element->attributes
            ->filter([html_qualified_name](nodes::attr* attribute) -> bool {return attribute->name == html_qualified_name;})
            .front();
}


dom::nodes::attr*
dom::helpers::attributes::get_attribute_by_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name,
        nodes::element* owner_element) {

    return owner_element->attributes
            ->filter([local_name, namespace_](nodes::attr* attribute) -> bool {return attribute->namespace_uri == namespace_ and attribute->local_name == local_name;})
            .front();
}


ext::string
dom::helpers::attributes::get_attribute_value(
        nodes::element* owner_element,
        ext::cstring& namespace_,
        ext::cstring& local_name) {

    auto* attribute = get_attribute_by_ns(namespace_, local_name, owner_element);
    return attribute->value;
}


dom::nodes::attr*
dom::helpers::attributes::set_attribute(
        nodes::attr* attribute,
        nodes::element* new_owner_element) {

    exceptions::throw_v8_exception(
            "attribute's element is non-null doesn't equal element",
            exceptions::INUSE_ATTRIBUTE_ERR,
            [attribute, new_owner_element] -> bool {return attribute->owner_element and attribute->owner_element == new_owner_element;});

    auto* old_attribute = get_attribute_by_ns(attribute->namespace_uri, attribute->local_name, new_owner_element);
    if (old_attribute == attribute) return nullptr;

    old_attribute
            ? replace(old_attribute, attribute)
            : append(attribute, new_owner_element);
    
    return old_attribute;

}


dom::nodes::attr*
dom::helpers::attributes::set_attribute_value(
        nodes::element* owner_element,
        ext::cstring& local_name,
        ext::cstring& value,
        ext::cstring& prefix,
        ext::cstring& namespace_) {

    auto* attribute = get_attribute_by_ns(namespace_, local_name, owner_element);
    if (not attribute) {
        attribute = new nodes::attr{};
        attribute->namespace_uri = namespace_;
        attribute->prefix = prefix;
        attribute->local_name = local_name;
        attribute->value = value;
        attribute->owner_document = owner_element->owner_document;
    }

    append(attribute, owner_element);
    change(attribute, value);
    return attribute;
}


dom::nodes::attr*
dom::helpers::attributes::remove_attribute_by_name(
        ext::cstring& qualified_name,
        nodes::element* owner_element) {

    auto* attribute = get_attribute_by_name(qualified_name, owner_element);
    if (attribute)
        remove(attribute);

    return attribute;
}


dom::nodes::attr*
dom::helpers::attributes::remove_attribute_by_ns(
        ext::cstring& namespace_,
        ext::cstring& local_name,
        nodes::element* owner_element) {

    auto* attribute = get_attribute_by_ns(namespace_, local_name, owner_element);
    if (attribute)
        remove(attribute);

    return attribute;
}


void
dom::helpers::attributes::set_existing_attribute_value(
        nodes::attr* attribute,
        ext::cstring& value) {

    if (not attribute->owner_element)
        attribute->value = value;
    else
        change(attribute, value);
}
