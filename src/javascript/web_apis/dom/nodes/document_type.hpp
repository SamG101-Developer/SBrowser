#pragma once
#ifndef SBROWSER_DOCUMENT_TYPE_HPP
#define SBROWSER_DOCUMENT_TYPE_HPP

#include <dom/nodes/node.hpp>
#include <dom/mixins/child_node.hpp>

namespace dom::nodes {class document_type;}
namespace dom::other {class dom_implementation;}


class dom::nodes::document_type
        : public node
        , public mixins::child_node<document_type>
{
public constructors:
    document_type();

public properties:
    ext::dom_property<ext::string> name;
    ext::dom_property<ext::string> public_id;
    ext::dom_property<ext::string> system_id;

public internal_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_DOCUMENT_TYPE_HPP
