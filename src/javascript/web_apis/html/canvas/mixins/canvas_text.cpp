#include "canvas_text.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template <typename T>
auto html::canvas::mixins::canvas_text<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_text<T>>{isolate}
            .template inherit<dom_object>()
            .template function("fillText", &canvas_text<T>::fill_text)
            .template function("strokeText", &canvas_text<T>::stroke_text)
            .template function("measureText", &canvas_text<T>::measure_text)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_text<html::canvas::canvas_rendering_context_2d>;
