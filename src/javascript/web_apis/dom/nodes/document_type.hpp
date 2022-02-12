#pragma once
#ifndef SBROWSER_DOCUMENT_TYPE_HPP
#define SBROWSER_DOCUMENT_TYPE_HPP

#include <dom/nodes/node.hpp>
#include <dom/mixins/child_node.hpp>

namespace dom::nodes {class document_type;}
namespace dom::other {class dom_implementation;}


class dom::nodes::document_type
        : public node
        , public mixins::child_node<document_type> {

public constructors:
    document_type();

public properties:
    ext::dom_property<ext::string, _F> name;
    ext::dom_property<ext::string, _F> public_id;
    ext::dom_property<ext::string, _F> system_id;

public: internal_methods
    ext::any v8(v8::Isolate *isolate) const override;
};


#endif //SBROWSER_DOCUMENT_TYPE_HPP
