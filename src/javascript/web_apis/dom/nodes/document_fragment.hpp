#pragma once
#ifndef SBROWSER_DOCUMENT_FRAGMENT_HPP
#define SBROWSER_DOCUMENT_FRAGMENT_HPP

#include <dom/nodes/node.hpp>
#include <dom/mixins/non_element_parent_node.hpp>
#include <dom/mixins/parent_node.hpp>

namespace dom::nodes {class document_fragment;}


class dom::nodes::document_fragment
        : public node
        , public mixins::non_element_parent_node<dom::nodes::document_fragment>
        , public mixins::parent_node<dom::nodes::document_fragment> {

public constructors:
    document_fragment();

public properties:
    ext::dom_property<dom::nodes::element*, _F> host;

private accessors:
    ext::string get_text_content() const override;

    void set_text_content(ext::cstring& val) override;
};


#endif //SBROWSER_DOCUMENT_FRAGMENT_HPP
