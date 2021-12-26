#include "dom_implementation.hpp"

#include <ext/string_switch.hpp>

#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/text.hpp>
#include <dom/nodes/xml_document.hpp>

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/namespaces.hpp>


dom::other::dom_implementation::dom_implementation() {
    m_associated_document = nullptr;
};


dom::other::dom_implementation::~dom_implementation() {
    delete m_associated_document;
}


dom::nodes::document_type*
dom::other::dom_implementation::create_document_type(
        ext::cstring& qualified_name,
        ext::cstring& public_id,
        ext::cstring& system_id) {

    helpers::namespaces::validate(qualified_name);

    auto* doctype = new nodes::document_type{};
    doctype->name = qualified_name;
    doctype->public_id = public_id;
    doctype->system_id = system_id;
    doctype->owner_document = m_associated_document;
    return doctype;
}


dom::nodes::xml_document*
dom::other::dom_implementation::create_document(
        ext::cstring& namespace_,
        ext::cstring& qualified_name,
        nodes::document_type* document_type) {

    auto* document = new nodes::xml_document{};
    auto* document_element = not qualified_name.empty()
            ? document->create_element_ns(namespace_, qualified_name)
            : nullptr;

    if (document_type) helpers::mutation_algorithms::append(document_type, document);
    if (document_element) helpers::mutation_algorithms::append(document_element, document);

    document->m_origin = m_associated_document->m_origin;

    string_switch(namespace_.c_str()) {
        string_case(helpers::namespaces::HTML): document->content_type = "application/xhtml+xml";
        string_case(helpers::namespaces::SVG): document->content_type = "image/svg+xml";
        string_default: document->content_type = "application/xml";
    }

    return document;
}


dom::nodes::document*
dom::other::dom_implementation::create_html_document(
        ext::cstring& title) {

    auto* document = new nodes::document{};
    document->m_type = "html";
    document->content_type = "text/html";

    auto* document_type = new nodes::document_type{};
    document_type->name = "html";
    document_type->owner_document = document;

    helpers::mutation_algorithms::append(document_type, document);
    helpers::mutation_algorithms::append(helpers::custom_elements::create_an_element(document, "html", helpers::namespaces::HTML), document);
    helpers::mutation_algorithms::append(helpers::custom_elements::create_an_element(document, "head", helpers::namespaces::HTML), document->child_nodes->at(1));

    if (not title.empty()) {
        auto* text_node = new nodes::text{title};
        text_node->owner_document = document;

        helpers::mutation_algorithms::append(helpers::custom_elements::create_an_element(document, "title", helpers::namespaces::HTML), document->child_nodes->at(2));
        helpers::mutation_algorithms::append(text_node, document->child_nodes->at(3));
    }

    helpers::mutation_algorithms::append(helpers::custom_elements::create_an_element(document, "body", helpers::namespaces::HTML), document->child_nodes->at(1));
    document->m_origin = m_associated_document->m_origin;
    return document;
}