#include "node_filter.hpp"


auto dom::iterators::node_filter::accept_node(nodes::node* node) const -> unsigned short {
    return 0x0; /* TODO : Find the impl or remove for legacy? */
}


auto dom::iterators::node_filter::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<node_filter>{isolate}
            .static_("FILTER_ACCEPT", node_filter::FILTER_ACCEPT)
            .static_("FILTER_SKIP", node_filter::FILTER_SKIP)
            .static_("FILTER_REJECT", node_filter::FILTER_REJECT)

            .static_("SHOW_ALL", node_filter::SHOW_ALL)
            .static_("SHOW_ELEMENT", node_filter::SHOW_ELEMENT)
            .static_("SHOW_ATTRIBUTE", node_filter::SHOW_ATTRIBUTE)
            .static_("SHOW_TEXT", node_filter::SHOW_TEXT)
            .static_("SHOW_CDATA_SECTION", node_filter::SHOW_CDATA_SECTION)
            .static_("SHOW_PROCESSING_INSTRUCTION", node_filter::SHOW_PROCESSING_INSTRUCTION)
            .static_("SHOW_COMMENT", node_filter::SHOW_COMMENT)
            .static_("SHOW_DOCUMENT", node_filter::SHOW_DOCUMENT)
            .static_("SHOW_DOCUMENT_TYPE", node_filter::SHOW_DOCUMENT_TYPE)
            .static_("SHOW_DOCUMENT_FRAGMENT", node_filter::SHOW_DOCUMENT_FRAGMENT)

            .function("acceptNode", &node_filter::accept_node)
            .auto_wrap_objects();
}
