#include "canvas_filter.hpp"


auto html::canvas::paint::canvas_filter::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_filter>{isolate}
            .ctor<canvas_filter_input_t>()
            .ctor<ext::vector<canvas_filter_input_t>>()
            .inherit<dom_object>()
            .auto_wrap_objects();
}
