#include "document_fragment.hpp"

#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/trees.hpp>


/*
 * https://dom.spec.whatwg.org/#dom-documentfragment-documentfragment
 * https://developer.mozilla.org/en-US/docs/Web/API/DocumentFragment/DocumentFragment
 *
 * The DocumentFragment() constructor returns a new, empty DocumentFragment object.
 */
dom::nodes::document_fragment::document_fragment()
        : node()
        , mixins::non_element_parent_node<document_fragment>()
        , mixins::parent_node<document_fragment>() {

    // set the properties
    host      << nullptr;
    node_type << DOCUMENT_FRAGMENT_NODE;
    node_name << "#document-fragment";
}


auto dom::nodes::document_fragment::get_text_content() const -> ext::string
{
    // the text content is the descendant text contents of this node
    return helpers::trees::descendant_text_content(this);
}


auto dom::nodes::document_fragment::set_text_content(const ext::string& val) -> void
{
    // replace all the text in this node with the new val
    helpers::node_internals::string_replace_all(val, this);
}


auto dom::nodes::document_fragment::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<document_fragment>{isolate}
            .ctor<>()
            .inherit<node>()
            .inherit<mixins::non_element_parent_node<document_fragment>>()
            .auto_wrap_objects();
}
