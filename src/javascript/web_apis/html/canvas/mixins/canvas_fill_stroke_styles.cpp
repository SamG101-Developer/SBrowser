#include "canvas_fill_stroke_styles.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template<typename T>
html::canvas::mixins::canvas_fill_stroke_styles<T>::canvas_fill_stroke_styles()
{
    // set the properties
    stroke_style << "black";
    fill_style << "black";
}


template<typename T>
auto html::canvas::mixins::canvas_fill_stroke_styles<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_fill_stroke_styles<T>>{isolate}
            .template inherit<dom_object>()
            .template function("createLinearGradient", &canvas_fill_stroke_styles<T>::create_linear_gradient)
            .template function("createRadialGradient", &canvas_fill_stroke_styles<T>::create_radial_gradient)
            .template function("createConicGradient", &canvas_fill_stroke_styles<T>::create_conic_gradient)
            .template function("createPattern", &canvas_fill_stroke_styles<T>::create_pattern)
            .template var("strokeStyle", &canvas_fill_stroke_styles<T>::stroke_style)
            .template var("fillStyle", &canvas_fill_stroke_styles<T>::fill_style)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_fill_stroke_styles<html::canvas::canvas_rendering_context_2d>;
