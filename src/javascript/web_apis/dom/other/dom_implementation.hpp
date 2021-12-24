#pragma once
#ifndef SBROWSER_DOM_IMPLEMENTATION_HPP
#define SBROWSER_DOM_IMPLEMENTATION_HPP

#include <ext/decorators.hpp>
#include <ext/string.hpp>

namespace dom {
    namespace other {class dom_implementation;}
    namespace nodes {
        class document_type;
        class document;
    }
}


class dom::other::dom_implementation {
public: constructors
    dom_implementation();
    ~dom_implementation();

public: methods
    nodes::document_type* create_document_type(ext::cstring& qualified_name, ext::cstring& public_id, ext::cstring& system_id);
    nodes::document* create_document(ext::cstring& namespace_, ext::cstring& qualified_name, nodes::document_type* document_type = nullptr);
    nodes::document* create_html_document(ext::cstring& title = "");

private: internal_properties
    nodes::document* m_associated_document;
};


#endif //SBROWSER_DOM_IMPLEMENTATION_HPP
