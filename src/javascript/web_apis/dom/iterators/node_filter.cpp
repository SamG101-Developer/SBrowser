#include "node_filter.hpp"


dom::iterators::node_filter::node_filter()
        : accept_node([](const nodes::node*) -> ushort {return FILTER_ACCEPT;})
{
}


auto dom::iterators::node_filter::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<node_filter>{isolate}
            .static_("FILTER_ACCEPT", node_filter::FILTER_ACCEPT, true)
            .static_("FILTER_SKIP", node_filter::FILTER_SKIP, true)
            .static_("FILTER_REJECT", node_filter::FILTER_REJECT, true)
            .static_("SHOW_ELEMENT", node_filter::SHOW_ELEMENT, true)
            .static_("SHOW_ATTRIBUTE", node_filter::SHOW_ATTRIBUTE, true)
            .static_("SHOW_TEXT", node_filter::SHOW_TEXT, true)
            .static_("SHOW_CDATA_SECTION", node_filter::SHOW_CDATA_SECTION, true)
            .static_("SHOW_PROCESSING_INSTRUCTION", node_filter::SHOW_PROCESSING_INSTRUCTION, true)
            .static_("SHOW_COMMENT", node_filter::SHOW_COMMENT, true)
            .static_("SHOW_DOCUMENT", node_filter::SHOW_DOCUMENT, true)
            .static_("SHOW_DOCUMENT_TYPE", node_filter::SHOW_DOCUMENT_TYPE, true)
            .static_("SHOW_DOCUMENT_FRAGMENT", node_filter::SHOW_DOCUMENT_FRAGMENT, true)
            .static_("SHOW_ALL", node_filter::SHOW_ALL, true)
            .var("acceptNode", &node_filter::accept_node, false)
            .auto_wrap_objects();
}
