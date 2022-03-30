#include "xml_document.hpp"


dom::nodes::xml_document::xml_document() = default;


auto dom::nodes::xml_document::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<xml_document>{isolate}
            .inherit<document>()
            .auto_wrap_objects();
}
