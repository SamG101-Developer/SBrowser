#pragma once
#ifndef SBROWSER_DOCUMENT_TYPE_HPP
#define SBROWSER_DOCUMENT_TYPE_HPP

#include <dom/nodes/node.hpp>

namespace dom::nodes {class document_type;}
namespace dom::other {class dom_implementation;}


class dom::nodes::document_type : public dom::nodes::node {
public constructors:
    document_type();

public properties:
    ext::dom_property<ext::string> name;
    ext::dom_property<ext::string> public_id;
    ext::dom_property<ext::string> system_id;
};


#endif //SBROWSER_DOCUMENT_TYPE_HPP
