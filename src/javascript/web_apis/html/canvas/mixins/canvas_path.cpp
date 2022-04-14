#include "canvas_path.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/paint/path_2d.hpp>


template <typename T>
auto html::canvas::mixins::canvas_path<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_path<T>>{isolate}
            .template inherit<dom_object>()
            .template function("close_path", &canvas_path<T>::close_path)
            .template function("move_to", &canvas_path<T>::move_to)
            .template function("line_to", &canvas_path<T>::line_to)
            .template function("quadratic_curve_to", &canvas_path<T>::quadratic_curve_to)
            .template function("bezier_curve_to", &canvas_path<T>::bezier_curve_to)
            .template function("arc_to", &canvas_path<T>::arc_to)
            .template function("rect", &canvas_path<T>::rect)
            .template function("round_rect", &canvas_path<T>::round_rect)
            .template function("arc", &canvas_path<T>::arc)
            .template function("ellpise", &canvas_path<T>::ellpise)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_path<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_path<html::canvas::paint::path_2d>;
