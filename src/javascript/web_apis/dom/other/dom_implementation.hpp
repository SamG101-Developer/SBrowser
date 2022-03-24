#pragma once
#ifndef SBROWSER_DOM_IMPLEMENTATION_HPP
#define SBROWSER_DOM_IMPLEMENTATION_HPP

#include <ext/decorators.hpp>
#include <ext/string.hpp>
#include <dom_object.hpp>

namespace dom::other {class dom_implementation;}
namespace dom::nodes {class document_type;}
namespace dom::nodes {class document;}
namespace dom::nodes {class xml_document;}


class dom::other::dom_implementation : virtual public dom_object
{
public: constructors
    dom_implementation();
    ~dom_implementation() override;

public: methods
    new_obj auto create_document_type(const ext::string& qualified_name, const ext::string& public_id, const ext::string& system_id) const -> nodes::document_type;
    new_obj auto create_document(const ext::string& namespace_, const ext::string& qualified_name, nodes::document_type* document_type = nullptr) const -> nodes::xml_document;
    new_obj auto create_html_document(const ext::string& title = "") const -> nodes::document;

public: internal_methods
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private: internal_properties
    nodes::document* m_associated_document = nullptr;
};


#endif //SBROWSER_DOM_IMPLEMENTATION_HPP
