#include "canvas_gradient.hpp"


auto html::canvas::paint::canvas_gradient::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_gradient>{isolate}
            .inherit<dom_object>()
            .function("addColorStop", &canvas_gradient::add_color_stop)
            .auto_wrap_objects();
}
