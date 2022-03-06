#pragma once
#ifndef SBROWSER_DOCUMENT_FRAGMENT_HPP
#define SBROWSER_DOCUMENT_FRAGMENT_HPP

#include <dom/nodes/node.hpp>
#include <dom/mixins/non_element_parent_node.hpp>
#include <dom/mixins/parent_node.hpp>

namespace dom::nodes {class document_fragment;}


/*
 * https://dom.spec.whatwg.org/#interface-documentfragment
 * https://developer.mozilla.org/en-US/docs/Web/API/DocumentFragment
 *
 * The DocumentFragment interface represents a minimal document object that has no parent.
 *
 * It is used as a lightweight version of Document that stores a segment of a document structure comprised of nodes just
 * like a standard document. The key difference is due to the fact that the document fragment isn't part of the active
 * document tree structure. Changes made to the fragment don't affect the document (even on reflow) or incur any
 * performance impact when changes are made.
 */
class dom::nodes::document_fragment
        : public node
        , public mixins::non_element_parent_node<dom::nodes::document_fragment>
        , public mixins::parent_node<dom::nodes::document_fragment>
{
public: constructors
    document_fragment();

public: properties
    ext::dom_property<dom::nodes::element*, _F> host;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private: accessors
    auto get_text_content() const -> ext::string override;
    auto set_text_content(const ext::string& val) -> void override;
};


#endif //SBROWSER_DOCUMENT_FRAGMENT_HPP
