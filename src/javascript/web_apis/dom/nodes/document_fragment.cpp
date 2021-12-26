#include "document_fragment.hpp"

#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/trees.hpp>


dom::nodes::document_fragment::document_fragment()
        : node()
        , mixins::non_element_parent_node<document_fragment>()
        , mixins::parent_node<document_fragment>() {

    node_type = DOCUMENT_FRAGMENT_NODE;
    node_name = "#document-fragment";
    host = nullptr;
}


ext::string dom::nodes::document_fragment::get_text_content() const {
    return helpers::trees::descendant_text_content(this);
}


void dom::nodes::document_fragment::set_text_content(ext::cstring& val) {
    helpers::node_internals::string_replace_all(this, val);
}
