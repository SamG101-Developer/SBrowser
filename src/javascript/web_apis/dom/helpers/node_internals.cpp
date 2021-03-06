#include "node_internals.hpp"

#include <regex>

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/namespaces.hpp>
#include <dom/helpers/ordered_sets.hpp>
#include <dom/helpers/shadows.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/attr.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/document_fragment.hpp>
#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/text.hpp>

#include <html/elements/html_element.hpp>
#include <html/helpers/browsing_context_internals.hpp>
#include <html/helpers/document_internals.hpp>
#include <html/helpers/custom_html_elements.hpp>


template <typename T>
auto dom::helpers::node_internals::clone(
        T* node,
        nodes::document* document,
        const bool deep)
        -> dom::nodes::node*
        requires std::is_base_of_v<T, nodes::node>
{
    // set the document to the node's document if the document is a nullptr, and create a pointer for the cloned_node
    document = document ? dynamic_cast<nodes::document*>(node->owner_document) : document;
    T* cloned_node;

    // if the node is or is derived an element, then set the clone to the result of the create_an_element helper method
    const auto* const element = dynamic_cast<nodes::element*>(node);
    cloned_node = element
            ? cloned_node = custom_elements::create_an_element(document, element->local_name, element->namespace_uri, element->prefix, element->m_is, false)
            : cloned_node = new T{*node};

    // if the node is or is derived an element, then clone the attributes over too
    if (element)
        element->attributes->for_each([node](auto* attribute) {mutation_algorithms::append(clone(attribute), node->owner_document);});

    // if the clone is a document, then set the owner document and document to copy, otherwise set the clones' owner
    // document to document
    if (not (cloned_node->owner_document = dynamic_cast<nodes::document*>(cloned_node)))
        cloned_node->owner_document = document;
    else
        document = cloned_node;

    cloned_node->m_behaviour.cloning_steps(node, document, deep);

    // if deep is set, then clone each child recursively, with the deep flag set
    if (deep)
        node->child_nodes->for_each([document](auto* child) {helpers::mutation_algorithms::append(clone(child, document, true));});

    // return the cloned node
    return cloned_node;
}


auto dom::helpers::node_internals::adopt(
        nodes::node* const node,
        const nodes::document* const document)
        -> void
{
    // get the old document
    const nodes::document* const old_document = node->owner_document;

    // if the node has a parent, then remove the node from its document, as it is being adopted into the new document
    if (node->parent)
        mutation_algorithms::remove(node);

    // if the two documents are different, the handle shadow options
    if (document != old_document)
    {
        for (nodes::node* inclusive_descendant: shadows::shadow_including_descendants(node))
        {
            inclusive_descendant->owner_document = document;
            if (auto* element = dynamic_cast<nodes::element*>(inclusive_descendant))
                element->attributes->for_each([document](nodes::attr* attribute) {attribute->owner_document = document;});
        }

        for (nodes::element* inclusive_descendant: shadows::shadow_including_descendants(node)
                .cast_all<nodes::element*>()
                .filter([](nodes::element* node) {return dom::helpers::custom_elements::is_custom_node(node);}))
            dom::helpers::custom_elements::enqueue_custom_element_callback_reaction(inclusive_descendant, "adoptedCallback", {old_document, document});

        for (nodes::node* inclusive_descendant: shadows::shadow_including_descendants(node))
            inclusive_descendant->m_behaviour.adopting_steps();
    }
}


auto dom::helpers::node_internals::locate_a_namespace_prefix(
        const nodes::element* const element,
        const ext::string& namespace_)
        -> ext::string
{
    // return the element prefix if the element namespace matches namespace_ and the element has a prefix
    if (element->namespace_uri == namespace_ and element->prefix)
        return element->prefix;

    // return the local name of the attribute, if an attribute whose value is the namespace_ and whose prefix is xmlns
    else if (const auto* const front = element->attributes->filter([namespace_](auto* const attribute) {return attribute->prefix == "xmlns" and attribute->value == namespace_;}).front())
        return front->local_name;

    // return the locating of a namespace prefix against the parent if the element has a parent
    else if (const nodes::element* const parent = element->parent_element)
        return locate_a_namespace_prefix(parent, namespace_);

    // return nullptr if all other checks fail
    return nullptr;
}


auto dom::helpers::node_internals::locate_a_namespace(
        const nodes::node* const node,
        const ext::string& prefix)
        -> ext::string
{
    // if the node is nullptr then return an empty namespace
    if (not node)
        return "";

    // otherwise, cast the node into an element and continue
    else if (auto* const element = dynamic_cast<const nodes::element*>(node))
    {
        // return the element's namespace if the namespace is present and the element's prefix matches the prefix
        if (element->namespace_uri and element->prefix == prefix)
            return element->namespace_uri;

        // return the value of the attribute, if the attribute exists where it is in the xmlns namespace, and either the
        // attribute's local name matched the prefix and the attribute's prefix is "xmlns"
        if (const auto* const front = element->attributes->first_match([prefix](const nodes::attr* const attribute) {return attribute->namespace_uri == namespaces::XMLNS and attribute->local_name == prefix and attribute->prefix == "xmlns";}))
            return front->value;

        // return the value of the attribute, if the attribute exists where it is in the xmlns namespace, and the
        // attribute's local name is "xmlns" and there is no attribute prefix or prefix
        if (const auto* const front = element->attributes->first_match([prefix](const nodes::attr* const attribute) {return attribute->namespace_uri == namespaces::XMLNS and attribute->local_name == "xmlns" and attribute->prefix == "" and prefix == "";}))
            return front->value;

        // return the locating of a namespace for the node's parent using prefix otherwise
        return node_internals::locate_a_namespace(node->parent, prefix);
    }

    // return the location of a namespace for the document element with prefix if the node is a document
    else if (auto* const document = dynamic_cast<const nodes::document*>(node))
        return locate_a_namespace(ext::property_dynamic_cast<nodes::node*>(document->document_element), prefix);

    // return nullptr if the node is a document_fragment or a document_type
    else if (::multi_cast<const nodes::document_fragment*, const nodes::document_type>(node))
        return nullptr;

    // return the location of a namespace for the owner document with prefix if the node is an attribute
    else if (auto* const attribute = dynamic_cast<const nodes::attr*>(node))
        return locate_a_namespace(ext::property_dynamic_cast<nodes::node*>(attribute->owner_document), prefix);

    // return a location for the namespace for the node's parent with prefix otherwise
    return locate_a_namespace(node->parent, prefix);
}


auto dom::helpers::node_internals::list_of_elements_with_qualified_name(
        const nodes::node* const node,
        const ext::string& qualified_name)
        -> ext::vector<dom::nodes::element*>
{
    // return all the elements, as the namespace and local name have no restrictions when the wild card option is used
    // for the qualified name
    if (qualified_name == "*")
        return trees::descendants(node).cast_all<nodes::element*>();

    // return a list of elements that match the qualified name, taking the html qualified name into account ie making it
    // lowercase if the document is a html document and the element is a html namespaced element
    return node->owner_document->m_type == "html"
            ? trees::descendants(node)
                .cast_all<nodes::element*>()
                .filter([qualified_name](const nodes::element* const descendant_element) {return descendant_element->m_qualified_name == (descendant_element->namespace_uri == "html" ? qualified_name.new_lowercase() : qualified_name);})

            : trees::descendants(node)
                .cast_all<nodes::element*>()
                .filter([qualified_name](const nodes::element* const descendant_element) {return descendant_element->m_qualified_name == qualified_name;});
}


auto dom::helpers::node_internals::list_of_elements_with_namespace_and_local_name(
        const nodes::node* const node,
        const ext::string& namespace_,
        const ext::string& local_name)
        -> ext::vector<dom::nodes::element*>
{
    // return all the elements, as the namespace and local name have no restrictions when the wild card option is used
    // for the namespace and local name
    if (namespace_ == "*" and local_name == "*")
        return trees::descendants(node)
            .cast_all<nodes::element*>();

    // return all the elements that have a matching local name to the given local name, as the namespace is still a
    // wildcard option
    else if (namespace_ == "*")
        return trees::descendants(node)
            .cast_all<nodes::element*>()
            .filter([namespace_](const nodes::element* const element) {return element->namespace_uri == namespace_;});

    // return all the elements that have a matching namespace to the given namespace, as the local name is still a
    // wildcard option
    else if (local_name == "*")
        return trees::descendants(node)
            .cast_all<nodes::element*>()
            .filter([local_name](const nodes::element* const element) {return element->local_name == local_name;});

    // return all the elements that have a matching namespace t the given namespace and a matching local name to the
    // given local name, as there are no wild card options
    else
        return trees::descendants(node)
            .cast_all<nodes::element*>()
            .filter([namespace_](const nodes::element* const element) {return element->namespace_uri == namespace_;})
            .filter([local_name](const nodes::element* const element) {return element->local_name == local_name;});
}


auto dom::helpers::node_internals::list_of_elements_with_class_names(
        const nodes::node* const node,
        const ext::string& class_names)
        -> ext::vector<dom::nodes::element*>
{
    // parse the classes into a set, and return an empty list if the set is empty
    const auto classes = ordered_sets::ordered_set_parser(class_names);
    if (classes.empty())
        return {};

    // TODO : comment; cba rn
    return trees::descendants(node)
            .cast_all<nodes::element*>()
            .filter([node, classes](const nodes::element* const descendant_element) {
                ext::string_vector class_list = *descendant_element->class_list;
                if (node->owner_document->m_mode == "quirks")
                    class_list.for_each([](ext::string& string) {string.to_lowercase();});

                return descendant_element->class_list->any_of([descendant_element](const ext::string& class_) {
                    return descendant_element->class_list->contains(class_);
                });
            });
}


auto dom::helpers::node_internals::string_replace_all(
        const ext::string& string,
        nodes::node* const parent)
        -> void
{
    // make sure that the string contains something
    if (not string.empty())
    {
        // create a new text node, set the data to the new string, and set the owner document to the parent's document
        auto* const text_node = new nodes::text{};
        text_node->data = string;
        text_node->owner_document = parent->owner_document;

        // replace all the nodes in parent with the text node, that contains the new text
        mutation_algorithms::replace_all(text_node, parent);
    }
}


auto dom::helpers::node_internals::is_document_fully_active(
        nodes::document* const document)
        -> bool
{
    // return true if the documents browsing context exists, the document is the active document, and [the container
    // document is active or there is no parent browsing context - auto enables document]
    return document->m_browsing_context
            and document->m_browsing_context->active_document() == document
            and (is_document_fully_active(document->m_browsing_context->container_document()) or not document->m_browsing_context->is_top_level_context());
}


auto dom::helpers::node_internals::is_html(
        const nodes::element* const element)
        -> bool
{
    // return if the element is in the html namespace, and the document's type is html
    return element->namespace_uri == namespaces::HTML and element->owner_document->m_type == "html";
}


template <typename ...nodes_or_strings>
auto dom::helpers::node_internals::convert_nodes_into_node(
        const nodes::document* const document,
        nodes_or_strings ...nodes)
        -> dom::nodes::node*
{
    // create an empty converted nodes list, and append a node if the object is a node, otherwise append a text node
    // containing the string as the data
    ext::vector<nodes::node*> converted_nodes{};
    ext::vector{nodes...}.template for_each([&converted_nodes](auto* node) {converted_nodes.append(dynamic_cast<nodes::node*>(node) ? node : std::unique_ptr<nodes::text>{node}.get());});

    // if there are nodes in the list, then return the first node
    if (not converted_nodes.empty())
        return converted_nodes.front();

    // otherwise, create a document fragment, append all the converted nodes to it, and return the document fragment
    const auto* const node = std::unique_ptr<nodes::document_fragment>{}.get();
    converted_nodes.template for_each([&node](const nodes::node* const converted_node) {helpers::mutation_algorithms::append(converted_node, node);});
    return (nodes::node*)node;
}
