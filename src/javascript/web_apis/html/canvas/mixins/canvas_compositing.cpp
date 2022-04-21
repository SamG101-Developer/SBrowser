#include "canvas_compositing.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/offscreen_canvas_rendering_context_2d.hpp>


template<typename T>
html::canvas::mixins::canvas_compositing<T>::canvas_compositing()
{
    // constrain the property values
    global_alpha.clamp_values(0.0, 1.0);
    global_compositing_operation.constrain_values(static_cast<T*>(this)->m_painter.translate_composition_mode.keys());

    // set the property values
    global_alpha = 1.0;
    global_compositing_operation = "source-over";
}


template<typename T>
auto html::canvas::mixins::canvas_compositing<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_compositing<T>>{isolate}
            .template var("globalAlpha", &canvas_compositing<T>::global_alpha)
            .template var("globalCompositingOperation", &canvas_compositing<T>::global_compositing_operation)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_compositing<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_compositing<html::canvas::offscreen_canvas_rendering_context_2d>;
