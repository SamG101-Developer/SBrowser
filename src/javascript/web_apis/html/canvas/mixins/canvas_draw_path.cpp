#include "canvas_draw_path.hpp"


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_draw_path<T>>{isolate}
            .template inherit<dom_object>()
            .template function("begin_path", &canvas_draw_path<T>::begin_path)
            .template function("fill", &canvas_draw_path<T>::fill)
            .template function("stroke", &canvas_draw_path<T>::stroke)
            .template function("clip", &canvas_draw_path<T>::clip)
            .template function("is_point_in_path", &canvas_draw_path<T>::is_point_in_path)
            .template function("is_point_in_stroke", &canvas_draw_path<T>::is_point_in_stroke)
            .auto_wrap_objects();
}
