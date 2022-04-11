#include "canvas_compositing.hpp"

#include <html/canvasing/canvas_rendering_context_2d.hpp>


template<typename T>
html::canvasing::mixins::canvas_compositing<T>::canvas_compositing()
{
    // set the properties
    global_alpha << 1.0;
    global_compositing_operation << "source-over";
}


template<typename T>
auto html::canvasing::mixins::canvas_compositing<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_compositing<T>>{isolate}
            .template var("globalAlpha", &canvas_compositing<T>::global_alpha)
            .template var("globalCompositingOperation", &canvas_compositing<T>::global_compositing_operation)
            .auto_wrap_objects();
}


template class html::canvasing::mixins::canvas_compositing<html::canvasing::canvas_rendering_context_2d>;
