#include "xslt_processor.hpp"


dom::other::xslt_processor::xslt_processor() = default;


auto dom::other::xslt_processor::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<xslt_processor>{isolate}
            .template ctor<>()
            .template inherit<dom_object>()
            .template function("importStylesheet", &xslt_processor::import_stylesheet)
            .template function("transformToFragment", &xslt_processor::transform_to_fragment)
            .template function("transformToDocument", &xslt_processor::transform_to_document)
            .template function("setParameter", &xslt_processor::set_parameter)
            .template function("getParameter", &xslt_processor::get_parameter)
            .template function("removeParameter", &xslt_processor::remove_parameter)
            .template function("clearParameters", &xslt_processor::clear_parameters)
            .template function("reset", &xslt_processor::reset)
            .auto_wrap_objects();
}
