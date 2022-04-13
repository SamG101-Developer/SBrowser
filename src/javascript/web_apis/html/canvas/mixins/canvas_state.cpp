#include "canvas_state.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template <typename T>
auto html::canvas::mixins::canvas_state<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_state<T>>{isolate}
            .template inherit<dom_object>()
            .template function("save", &canvas_state<T>::save)
            .template function("restore", &canvas_state<T>::restore)
            .template function("reset", &canvas_state<T>::reset)
            .template function("isContextLot", &canvas_state<T>::is_context_lost)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_state<html::canvas::canvas_rendering_context_2d>;
