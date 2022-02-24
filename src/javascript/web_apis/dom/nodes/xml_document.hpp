#pragma once
#ifndef SBROWSER_XML_DOCUMENT_HPP
#define SBROWSER_XML_DOCUMENT_HPP

#include <dom/nodes/document.hpp>

namespace dom::nodes {class xml_document;}


class dom::nodes::xml_document : public nodes::document
{
public: constructors
    xml_document();

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_XML_DOCUMENT_HPP
