#include "canvas_shadow_styles.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template <typename T>
html::canvas::mixins::canvas_shadow_styles<T>::canvas_shadow_styles()
{
    // set the property values
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
    return v8pp::class_<canvas_shadow_styles<T>>{isolate}
            .template inherit<dom_object>()
            .template var("shadowOffsetX", &canvas_shadow_styles<T>::shadow_offset_x)
            .template var("shadowOffsetY", &canvas_shadow_styles<T>::shadow_offset_y)
            .template var("shadowBlur", &canvas_shadow_styles<T>::shadow_blur)
            .template var("shadowColor", &canvas_shadow_styles<T>::shadow_color)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_shadow_styles<html::canvas::canvas_rendering_context_2d>;
