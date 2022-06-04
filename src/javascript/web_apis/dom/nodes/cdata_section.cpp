#include "cdata_section.hpp"


dom::nodes::cdata_section::cdata_section()
        : text()
{
    // set the property's values
    node_type = CDATA_SECTION_NODE;
    node_name = "#cdata-section";
}


auto dom::nodes::cdata_section::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<cdata_section>{isolate}
            .inherit<text>()
            .auto_wrap_objects();
}
