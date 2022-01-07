#include "node_filter.hpp"


ext::any&&
dom::iterators::node_filter::v8(v8::Isolate* isolate) const {

    return v8pp::class_<dom::iterators::node_filter>{isolate}
            .static_("FILTER_ACCEPT", dom::iterators::node_filter::FILTER_ACCEPT)
            .static_("FILTER_SKIP", dom::iterators::node_filter::FILTER_SKIP)
            .static_("FILTER_REJECT", dom::iterators::node_filter::FILTER_REJECT)

            .static_("SHOW_ALL", dom::iterators::node_filter::SHOW_ALL)
            .static_("SHOW_ELEMENT", dom::iterators::node_filter::SHOW_ELEMENT)
            .static_("SHOW_ATTRIBUTE", dom::iterators::node_filter::SHOW_ATTRIBUTE)
            .static_("SHOW_TEXT", dom::iterators::node_filter::SHOW_TEXT)
            .static_("SHOW_CDATA_SECTION", dom::iterators::node_filter::SHOW_CDATA_SECTION)
            .static_("SHOW_PROCESSING_INSTRUCTION", dom::iterators::node_filter::SHOW_PROCESSING_INSTRUCTION)
            .static_("SHOW_COMMENT", dom::iterators::node_filter::SHOW_COMMENT)
            .static_("SHOW_DOCUMENT", dom::iterators::node_filter::SHOW_DOCUMENT)
            .static_("SHOW_DOCUMENT_TYPE", dom::iterators::node_filter::SHOW_DOCUMENT_TYPE)
            .static_("SHOW_DOCUMENT_FRAGMENT", dom::iterators::node_filter::SHOW_DOCUMENT_FRAGMENT)

            .function("acceptNode", &dom::iterators::node_filter::accept_node)
            .auto_wrap_objects();
}
