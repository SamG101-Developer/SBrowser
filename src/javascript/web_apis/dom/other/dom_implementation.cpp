#include "dom_implementation.hpp"

#include <ext/macros//string_switch.hpp>

#include <dom/nodes/document_type.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/text.hpp>
#include <dom/nodes/xml_document.hpp>

#include <dom/helpers/custom_elements.hpp>
#include <dom/helpers/mutation_algorithms.hpp>
#include <dom/helpers/namespaces.hpp>


dom::other::dom_implementation::dom_implementation()
{
    // set the attributes
    m_associated_document = nullptr;
};


dom::other::dom_implementation::~dom_implementation()
{
    // delete the document associated with this dom implementation when it is destroyed
    delete m_associated_document;
}


auto dom::other::dom_implementation::create_document_type(
        const ext::string& qualified_name,
        const ext::string& public_id,
        const ext::string& system_id)
        -> dom::nodes::document_type*
{
    // check that the qualified name is valid
    helpers::namespaces::validate(qualified_name);

    // create a doctype node, and set the owner document to this dom implementation's associated document
    auto* doctype = new nodes::document_type{};
    doctype->name = qualified_name;
    doctype->public_id = public_id;
    doctype->system_id = system_id;
    doctype->owner_document = m_associated_document;
    return doctype;
}


auto dom::other::dom_implementation::create_document(
        const ext::string& namespace_,
        const ext::string& qualified_name,
        nodes::document_type* document_type)
        -> dom::nodes::xml_document*
{
    // create a new non-html document, and set the document element to a created element if a qualified name is given
    auto* document = new nodes::xml_document{};
    auto* document_element = not qualified_name.empty()
            ? document->create_element_ns(namespace_, qualified_name)
            : nullptr;

    // if a doctype is given, append it to the document
    if (document_type)
        helpers::mutation_algorithms::append(document_type, document);

    // if a document element is given, append it to the document
    if (document_element)
        helpers::mutation_algorithms::append(document_element, document);

    // set the document's origin to the associated document's origin
    document->m_origin = m_associated_document->m_origin;

    //switch on the namespace to set the document's content type
    string_switch(namespace_)
    {
        string_case(helpers::namespaces::HTML):
            document->content_type = "application/xhtml+xml";

        string_case(helpers::namespaces::SVG):
            document->content_type = "image/svg+xml";

        string_default:
            document->content_type = "application/xml";
    }

    // return the created document
    return document;
}


auto dom::other::dom_implementation::create_html_document(const ext::string& title) -> dom::nodes::document*
{
    // create a new document, and set its type and content type to html
    auto* document = new nodes::document{};
    document->m_type = "html";
    document->content_type = "text/html";

    // create a new doctype and set its document to the new html document
    auto* document_type = new nodes::document_type{};
    document_type->name = "html";
    document_type->owner_document = document;

    // append the doctype into the document, and create a new html and head element
    helpers::mutation_algorithms::append(document_type, document);
    helpers::mutation_algorithms::append(helpers::custom_elements::create_an_element(document, "html", helpers::namespaces::HTML), document);
    helpers::mutation_algorithms::append(helpers::custom_elements::create_an_element(document, "head", helpers::namespaces::HTML), document->child_nodes->at(1));

    if (not title.empty()) {
        // create a new text node with the title text in, and set the owner document to the new html document
        auto* text_node = new nodes::text{title};
        text_node->owner_document = document;

        // append the title element into the document head and append the text node into the title element
        helpers::mutation_algorithms::append(helpers::custom_elements::create_an_element(document, "title", helpers::namespaces::HTML), document->child_nodes->at(2));
        helpers::mutation_algorithms::append(text_node, document->child_nodes->at(3));
    }

    // create a new body node and add it to the html element
    helpers::mutation_algorithms::append(helpers::custom_elements::create_an_element(document, "body", helpers::namespaces::HTML), document->child_nodes->at(1));

    // set the document's origin to the associated document's origin, and return the document
    document->m_origin = m_associated_document->m_origin;
    return document;
}


auto dom::other::dom_implementation::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<dom_implementation>{isolate}
            .function("createDocumentType", &dom_implementation::create_document_type)
            .function("createDocument", &dom_implementation::create_document)
            .function("createHTMLDocument", &dom_implementation::create_html_document)
            .auto_wrap_objects();
}
