#include "node_filter.hpp"


dom::iterators::node_filter::node_filter()
{
    // set the properties
    accept_node = [](const nodes::node* node) {return FILTER_ACCEPT;};
}


auto dom::iterators::node_filter::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<node_filter>{isolate}
            .template static_("FILTER_ACCEPT", node_filter::FILTER_ACCEPT)
            .template static_("FILTER_SKIP", node_filter::FILTER_SKIP)
            .template static_("FILTER_REJECT", node_filter::FILTER_REJECT)
            .template static_("SHOW_ALL", node_filter::SHOW_ALL)
            .template static_("SHOW_ELEMENT", node_filter::SHOW_ELEMENT)
            .template static_("SHOW_ATTRIBUTE", node_filter::SHOW_ATTRIBUTE)
            .template static_("SHOW_TEXT", node_filter::SHOW_TEXT)
            .template static_("SHOW_CDATA_SECTION", node_filter::SHOW_CDATA_SECTION)
            .template static_("SHOW_PROCESSING_INSTRUCTION", node_filter::SHOW_PROCESSING_INSTRUCTION)
            .template static_("SHOW_COMMENT", node_filter::SHOW_COMMENT)
            .template static_("SHOW_DOCUMENT", node_filter::SHOW_DOCUMENT)
            .template static_("SHOW_DOCUMENT_TYPE", node_filter::SHOW_DOCUMENT_TYPE)
            .template static_("SHOW_DOCUMENT_FRAGMENT", node_filter::SHOW_DOCUMENT_FRAGMENT)
            .template var("acceptNode", &node_filter::accept_node, false)
            .auto_wrap_objects();
}
