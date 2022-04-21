#include "canvas_draw_path.hpp"

#include <html/canvas/paint/path_2d.hpp>
#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/offscreen_canvas_rendering_context_2d.hpp>

#include <html/helpers/canvas_internals.hpp>


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::begin_path()
        -> void
{
    // clear the current default path of any subpaths
    m_current_default_path.clear();
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::fill(
        const ext::string& fill_rule)
        -> void
{
    // run the fill steps with no path
    fill(nullptr, fill_rule);
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::fill(
        paint::path_2d* path,
        const ext::string& fill_rule)
        -> void
{
    // run the fill steps with the path
    auto* object = static_cast<T*>(this);
    path->m_path.template for_each([&object](const sub_path_t& sub_path) {object->m_painter.fill_path(sub_path.qt_painter_subpath, object->fill_style);});
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::stroke()
        -> void
{
    // run the stroke steps with no path
    stroke(nullptr);
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::stroke(
        paint::path_2d* path)
        -> void
{
    // run the stroke steps with the path
    auto* object = static_cast<T*>(this);
    path->m_path.template for_each([&object](const sub_path_t& sub_path) {object->m_painter.stroke_path(sub_path.qt_painter_subpath, object->stroke_style);});
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::clip(
        const ext::string& fill_rule)
        -> void
{
    // run the clip steps with no path
    clip(nullptr, fill_rule);
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::clip(
        paint::path_2d* path,
        const ext::string& fill_rule)
        -> void
{
    // run the clip steps with the path
    // TODO
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::is_point_in_path(
        const double x,
        const double y,
        const ext::string& fill_rule)
        -> bool
{
    // run the is_point_in_path steps with no path
    return is_point_in_path(nullptr, x, y, fill_rule);
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::is_point_in_path(
        paint::path_2d* path,
        const double x,
        const double y,
        const ext::string& fill_rule)
        -> bool
{
    // TODO
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::is_point_in_stroke(
        const double x,
        const double y)
        -> bool
{
    // run the is_point_in_stroke steps with no path
    return is_point_in_stroke(nullptr, x, y);
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::is_point_in_stroke(
        paint::path_2d* path,
        double x,
        double y)
        -> bool
{
    // TODO
}


template <typename T>
auto html::canvas::mixins::canvas_draw_path<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{ // TODO -> types for overloads
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


template class html::canvas::mixins::canvas_draw_path<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_draw_path<html::canvas::offscreen_canvas_rendering_context_2d>;
