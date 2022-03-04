#pragma once
#ifndef SBROWSER_DOM_IMPLEMENTATION_HPP
#define SBROWSER_DOM_IMPLEMENTATION_HPP

#include <ext/macros/decorators.hpp>
#include <ext/iterables/string.hpp>

#include <dom_object.hpp>

namespace dom
{
    namespace other {class dom_implementation;}
    namespace nodes
    {
        class document_type;
        class document;
        class xml_document;
    }
}


class dom::other::dom_implementation : virtual public dom_object
{
public: constructors
    dom_implementation();
    ~dom_implementation() override;

public: methods
    auto create_document_type(const ext::string& qualified_name, const ext::string& public_id, const ext::string& system_id) -> nodes::document_type*;
    auto create_document(const ext::string& namespace_, const ext::string& qualified_name, nodes::document_type* document_type = nullptr) -> nodes::xml_document*;
    auto create_html_document(const ext::string& title = "") -> nodes::document*;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private: internal_properties
    nodes::document* m_associated_document;
};


#endif //SBROWSER_DOM_IMPLEMENTATION_HPP
