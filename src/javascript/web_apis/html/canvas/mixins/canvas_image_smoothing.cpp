#include "canvas_image_smoothing.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/offscreen_canvas_rendering_context_2d.hpp>

#include <render/painting/painter.hpp>


template<typename T>
html::canvas::mixins::canvas_image_smoothing<T>::canvas_image_smoothing()
{
    // constrain the property values
    image_smoothing_quality.constrain_values(static_cast<T*>(this)->m_painter.translate_image_smoothing_quality.keys());

    // attach the qt functions
    image_smoothing_enabled.template attach_qt_updater(&render::painting::painter::set_image_smoothing_enabled, static_cast<T*>(this)->m_painter);
    image_smoothing_quality.template attach_qt_updater(&render::painting::painter::set_image_smoothing_quality, static_cast<T*>(this)->m_painter);

    // set the property's values
    image_smoothing_enabled = true;
    image_smoothing_quality = "low";
}


template<typename T>
auto html::canvas::mixins::canvas_image_smoothing<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_image_smoothing<T>>{isolate}
            .template inherit<dom_object>()
            .template var("imageSmoothingEnabled", &canvas_image_smoothing<T>::image_smoothing_enabled)
            .template var("imageSmoothingQuality", &canvas_image_smoothing<T>::image_smoothing_quality)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_image_smoothing<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_image_smoothing<html::canvas::offscreen_canvas_rendering_context_2d>;
