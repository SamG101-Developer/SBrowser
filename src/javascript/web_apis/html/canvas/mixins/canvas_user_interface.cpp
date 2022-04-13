#include "canvas_user_interface.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template <typename T>
auto html::canvas::mixins::canvas_user_interface<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_user_interface<T>>{isolate}
            .template function<void(canvas_user_interface<T>::*)(dom::nodes::element*)>("drawFocusIfNeeded", &canvas_user_interface<T>::draw_focus_if_needed)
            .template function<void(canvas_user_interface<T>::*)(paint::path_2d*, dom::nodes::element*)>("drawFocusIfNeeded", &canvas_user_interface<T>::draw_focus_if_needed)
            .template function<void(canvas_user_interface<T>::*)()>("scrollPathIntoView", &canvas_user_interface<T>::scroll_path_into_view)
            .template function<void(canvas_user_interface<T>::*)(paint::path_2d*)>("scrollPathIntoView", &canvas_user_interface<T>::scroll_path_into_view)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_user_interface<html::canvas::canvas_rendering_context_2d>;
