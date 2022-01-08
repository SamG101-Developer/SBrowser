#include "xslt_processor.hpp"


dom::other::xslt_processor::xslt_processor()
        : dom_object() {
}


ext::any
dom::other::xslt_processor::v8(v8::Isolate* isolate) const {

    return v8pp::class_<xslt_processor>{isolate}
            .ctor<>()

            .function("importStylesheet", &xslt_processor::import_stylesheet)
            .function("transformToFragment", &xslt_processor::transform_to_fragment)
            .function("transformToDocument", &xslt_processor::transform_to_document)
            .function("setParameter", &xslt_processor::set_parameter)
            .function("getParameter", &xslt_processor::get_parameter)
            .function("removeParameter", &xslt_processor::remove_parameter)
            .function("clearParameters", &xslt_processor::clear_parameters)
            .function("reset", &xslt_processor::reset)

            .auto_wrap_objects();
}
