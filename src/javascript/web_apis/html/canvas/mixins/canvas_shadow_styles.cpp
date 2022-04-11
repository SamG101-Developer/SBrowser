#include "canvas_shadow_styles.hpp"


template <typename T>
html::canvas::mixins::canvas_shadow_styles<T>::canvas_shadow_styles()
{
    // set the properties
    shadow_offset_x << 0.0;
    shadow_offset_y << 0.0;
    shadow_blur << 0.0;
    shadow_color << "black";
}


template <typename T>
auto html::canvas::mixins::canvas_shadow_styles<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_shadow_styles<T>>{isolate};
}
