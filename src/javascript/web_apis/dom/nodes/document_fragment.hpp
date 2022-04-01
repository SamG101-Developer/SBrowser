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
        , public mixins::parent_node<dom::nodes::document_fragment>
{
public constructors:
    document_fragment();

public js_properties:
    ext::dom_property<dom::nodes::element*> host;

public cpp_methods:
    auto qt() const -> QWidget* override {return nullptr;}
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private accessors:
    auto get_text_content() const -> ext::string override;
    auto set_text_content(const ext::string& val) -> void override;
};


#endif //SBROWSER_DOCUMENT_FRAGMENT_HPP
