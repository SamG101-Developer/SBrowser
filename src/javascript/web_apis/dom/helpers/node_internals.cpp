#include "node_internals.hpp"

#include <regex>

#include <ext/iterables/set.hpp>

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/ordered_sets.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/text.hpp>

#include <html/elements/html_element.hpp>
#include <html/helpers/document_internals.hpp>


template <typename T>
dom::nodes::node*
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
        element->attributes->for_each([node](auto* attribute) {mutation_algorithms::append(clone(attribute), node->owner_document);});
    if (not (cloned_node->owner_document = dynamic_cast<nodes::document*>(cloned_node)))
        cloned_node->owner_document = document;
    if (deep)
        node->child_nodes->for_each([document](auto* child) {helpers::mutation_algorithms::append(clone(child, document, true));});

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

        if (auto* front = element->attributes->filter([prefix](nodes::attr* attribute) {return (attribute->namespace_uri == namespaces::XMLNS and attribute->local_name == prefix and attribute->prefix == "xmlns") or (attribute->namespace_uri == namespaces::XMLNS and attribute->local_name == "xmlns" and not attribute->prefix and not prefix);}).front())
            return front->value;

        return node_internals::locate_a_namespace(node, prefix);
    }

    else if (auto* document = dynamic_cast<nodes::document*>(node))
        return locate_a_namespace(ext::property_dynamic_cast<nodes::node*>(document->document_element), prefix);

    else if (dynamic_cast<nodes::document_fragment*>(node) or dynamic_cast<nodes::document_type*>(node))
        return nullptr;

    else if (auto* attribute = dynamic_cast<nodes::attr*>(node))
        return locate_a_namespace(ext::property_dynamic_cast<nodes::node*>(attribute->owner_document), prefix);

    return locate_a_namespace(node->parent, prefix);
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
                .filter([qualified_name](nodes::element* descendant_element) {return descendant_element->m_qualified_name == (descendant_element->namespace_uri == "html" ? qualified_name.new_lowercase() : qualified_name);})

            : trees::descendants(node)
                .cast_all<nodes::element*>()
                .filter([qualified_name](nodes::element* descendant_element) {return descendant_element->m_qualified_name == qualified_name;});
}


ext::vector<dom::nodes::element*>
dom::helpers::node_internals::list_of_elements_with_namespace_and_local_name(
        nodes::node* node,
        ext::cstring& namespace_,
        ext::cstring& local_name) {

    return trees::descendants(node)
            .cast_all<nodes::element*>()
            .filter([namespace_, local_name](nodes::element* descendant_element) {
                return std::regex_match(descendant_element->namespace_uri->c_str(), std::regex(namespace_.c_str())) and
                       std::regex_match(descendant_element->local_name->c_str(), std::regex(local_name.c_str()));});
}


ext::vector<dom::nodes::element*>
dom::helpers::node_internals::list_of_elements_with_class_names(
        nodes::node* node,
        ext::cstring& class_names) {

    auto classes = ordered_sets::ordered_set_parser(class_names);
    if (classes.empty())
        return {};

    return trees::descendants(node)
            .cast_all<nodes::element*>()
            .filter([node, classes](nodes::element* descendant_element) {
                ext::vector<ext::string> class_list = *descendant_element->class_list;
                if (node->owner_document->m_mode == "quirks")
                    class_list.for_each([](ext::string& string) {string.to_lowercase();});

                return descendant_element->class_list->any_of([descendant_element](ext::cstring& class_) {
                    return descendant_element->class_list->contains(class_);
                });
            });
}


void
dom::helpers::node_internals::adopt(
        nodes::node* node,
        nodes::document* document) {

    nodes::document* old_document = node->owner_document;
    if (node->parent) mutation_algorithms::remove(node);
    if (document != old_document) /* TODO show inclusive descendants */ return;
}


void
dom::helpers::node_internals::string_replace_all(
        ext::cstring& string,
        nodes::node* parent) {

    if (not string.empty()) {
        auto* text_node = new nodes::text{};
        text_node->data = string;
        text_node->owner_document = parent->owner_document;
        mutation_algorithms::replace_all(text_node, parent);
    }
}


bool
dom::helpers::node_internals::is_document_fully_active(
        nodes::document* document) {

    return document->m_browsing_context
            and document->m_browsing_context->active_document == document
            and (is_document_fully_active(document->m_browsing_context->container_document) or not document->m_browsing_context->parent_browsing_context);
}


bool
dom::helpers::node_internals::is_html(
        nodes::element* element) {

    return element->namespace_uri == "html" and element->owner_document->m_type == "html";
}


ext::string
dom::helpers::node_internals::advisory_information(
        html::elements::html_element* element) {

    return element->title ? element->title : element->parent_element
            ? advisory_information(ext::property_dynamic_cast<html::elements::html_element*>(element->parent_element))
            : "";
}


template <typename ...nodes_or_strings>
dom::nodes::node*
dom::helpers::node_internals::convert_nodes_into_node(
        nodes::document* document,
        nodes_or_strings ...nodes) {

    ext::vector<nodes::node*> converted_nodes{};
    ext::vector{nodes...}.template for_each([&converted_nodes](auto* node) {converted_nodes.append(dynamic_cast<nodes::node*>(node) ? node : new nodes::text{node});});

    auto* node = converted_nodes.front();
    if (node) return node;

    node = new nodes::document_fragment{};
    converted_nodes.template for_each([&master_node = node](auto* node) {helpers::mutation_algorithms::append(node, master_node);});
    return node;
}
