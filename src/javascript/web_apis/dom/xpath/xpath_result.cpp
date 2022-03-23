#include "xpath_result.hpp"


dom::xpath::xpath_result::xpath_result(): dom_object()
{
}


auto dom::xpath::xpath_result::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<xpath_result>{isolate}
            .template inherit<dom_object>()
            .template static_("ANY_TYPE", xpath_result::ANY_TYPE)
            .template static_("NUMBER_TYPE", xpath_result::NUMBER_TYPE)
            .template static_("STRING_TYPE", xpath_result::STRING_TYPE)
            .template static_("BOOLEAN_TYPE", xpath_result::BOOLEAN_TYPE)
            .template static_("UNORDERED_NODE_ITERATOR_TYPE", xpath_result::UNORDERED_NODE_ITERATOR_TYPE)
            .template static_("ORDERED_NODE_ITERATOR_TYPE", xpath_result::ORDERED_NODE_ITERATOR_TYPE)
            .template static_("UNORDERED_NODE_SNAPSHOT_TYPE", xpath_result::UNORDERED_NODE_SNAPSHOT_TYPE)
            .template static_("ORDERED_NODE_SNAPSHOT_TYPE", xpath_result::ORDERED_NODE_SNAPSHOT_TYPE)
            .template static_("ANY_UNORDERED_NODE_TYPE", xpath_result::ANY_UNORDERED_NODE_TYPE)
            .template static_("FIRST_ORDERED_NODE_TYPE", xpath_result::FIRST_ORDERED_NODE_TYPE)
            .template function("iteratorNext", &xpath_result::iterate_next)
            .template function("snapshotLength", &xpath_result::snapshot_item)
            .template var("resultType", &xpath_result::result_type)
            .template var("numberValue", &xpath_result::number_value)
            .template var("stringValue", &xpath_result::string_value)
            .template var("booleanValue", &xpath_result::boolean_value)
            .template var("singleNodeValue", &xpath_result::single_node_value)
            .template var("invalidIteratorState", &xpath_result::invalid_iterator_state)
            .template var("snapshotLength", &xpath_result::snapshot_length)
            .auto_wrap_objects();
}
