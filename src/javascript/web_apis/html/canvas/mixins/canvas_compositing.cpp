#include "canvas_compositing.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template<typename T>
html::canvas::mixins::canvas_compositing<T>::canvas_compositing()
{
    // set the property values
    global_alpha << 1.0;
    global_compositing_operation << "source-over";
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
