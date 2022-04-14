#include "canvas_pattern.hpp"


auto html::canvas::paint::canvas_pattern::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_pattern>{isolate}
            .inherit<dom_object>()
            .function("setTransform", &canvas_pattern::set_transform)
            .auto_wrap_objects();
}
