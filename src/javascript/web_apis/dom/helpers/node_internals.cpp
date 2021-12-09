#include "node_internals.hpp"

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/namespaces.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>


template <typename T> void
dom::helpers::node_internals::clone(
        T* node,
        nodes::document* document,
        bool deep) requires std::is_base_of_v<T, nodes::node> {

    document = document ? dynamic_cast<nodes::document*>(node->owner_document) : document;
    T* cloned_node;

    auto* element = dynamic_cast<nodes::element*>(node);
    cloned_node = (element)
            ? cloned_node = custom_elements::create_an_element(document, element->local_name, element->namespace_uri, element->prefix, element->m_is, false)
            : cloned_node = new T{*node};

    if (element)
        element->attributes->for_each([node](auto* attribute) -> void {mutation_algorithms::append(clone(attribute), node->owner_document);});
    if (not cloned_node->owner_document = dynamic_cast<nodes::document*>(cloned_node))
        cloned_node->owner_document = document;
    if (deep)
        node->child_nodes->for_each([document](auto* child) -> void {helpers::mutation_algorithms::append(clone(child, document, true));});

    return cloned_node;
}


ext::string
dom::helpers::node_internals::locate_a_namespace_prefix(
        nodes::element* element,
        ext::cstring& namespace_) {

    if (element->namespace_uri == namespace_ and element->prefix)
        return element->prefix;

    else if (auto front = element->attributes->filter([namespace_](auto* attribute) {return attribute->prefix == "xmlns" and attribute->value == namespace_;}).front())
        return front->name;

    else if (nodes::element* parent = element->parent_element)
        return locate_a_namespace_prefix(parent, namespace_);

    return nullptr;
}


ext::string
dom::helpers::node_internals::locate_a_namespace(
        nodes::node* node,
        ext::cstring& prefix) {

    if (not node)
        return "";

    else if (auto* element = dynamic_cast<nodes::element*>(node)) {
        if (element->namespace_uri and element->prefix == prefix)
            return element->namespace_uri;

        if (auto* front = element->attributes->filter([prefix](nodes::attr* attribute) -> bool {return (attribute->namespace_uri == namespaces::XMLNS and attribute->local_name == prefix and attribute->prefix == "xmlns") or (attribute->namespace_uri == namespaces::XMLNS and attribute->local_name == "xmlns" and not attribute->prefix and not prefix);}).front())
            return front->value;

        return node_internals::locate_a_namespace(node, prefix);
    }

    else if (auto* document = dynamic_cast<nodes::document*>(node))
        return locate_a_namespace(ext::property_dynamic_cast<nodes::node>(document->document_element), prefix);

    else if (dynamic_cast<nodes::document_fragment*>(node) or dynamic_cast<nodes::document_type*>(node))
        return nullptr;

    else if (auto* attribute = dynamic_cast<nodes::attr*>(node))
        return locate_a_namespace(ext::property_dynamic_cast<nodes::node*>(attribute->owner_document), prefix);

    return locate_a_namespace(node->parent_node, prefix);
}


ext::vector<dom::nodes::element*>
dom::helpers::node_internals::list_of_elements_with_qualified_name(
        nodes::node* node,
        ext::cstring& qualified_name) {

    if (qualified_name == "*")
        return trees::descendants(node).cast_all<nodes::element*>();

    return node->owner_document->m_type == "html"
            ? trees::descendants(node)
                .cast_all<nodes::element*>()
                .filter([qualified_name](auto* descendant_element) -> bool {return descendant_element->namespace_uri == "html" and descendant_element->m_qualified_name == qualified_name.new_lower_case() or descendant_element->namespace_uri != "html" and descendant_element->m_qualified_name == qualified_name;})

            : trees::descendants(node)
                .cast_all<nodes::element*>()
                .filter([qualified_name](auto* descendant_element) -> bool {return descendant_element->m_qualified_name == qualified_name;});
}
