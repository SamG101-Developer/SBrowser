#include "canvas_path_drawing_styles.hpp"

#include <ext/infinity.hpp>
#include <ext/string_switch.hpp>

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/offscreen_canvas_rendering_context_2d.hpp>


template <typename T>
html::canvas::mixins::canvas_path_drawing_styles<T>::canvas_path_drawing_styles()
{
    // constrain the property values
    line_cap.constrain_values(m_pen.translate_cap_style.keys());
    line_join.constrain_values(m_pen.translate_join_style.keys());

    // attach the qt functions
    line_width.template attach_qt_updater(&render::painting::pen::set_pen_width, m_pen);
    miter_limit.template attach_qt_updater(&render::painting::pen::set_miter_limit, m_pen);
    line_cap.template attach_qt_updater(&render::painting::pen::set_cap_style, m_pen);
    line_join.template attach_qt_updater(&render::painting::pen::set_join_style, m_pen);

    // set the property's values
    line_width = 1.0;
    miter_limit = 10.0;
    line_cap = "butt";
    line_join = "miter";
}


template <typename T>
auto html::canvas::mixins::canvas_path_drawing_styles<T>::set_line_dash(
        const ext::vector<double>& segments)
        -> void
{
    // return if any of the segment values are negative, infinite, or NaN (TODO : NaN and report/debug to console)
    if (segments.template any_of([](double&& segment) {return segment == ext::infinity<double>{} or segment < 0;}))
        return;

    // set the dash list to the segments, duplicating it if there are an odd number of values
    m_dash_list = segments;
    if (segments.length() % 2 == 1)
        m_dash_list.extend(m_dash_list);
}


template <typename T>
auto html::canvas::mixins::canvas_path_drawing_styles<T>::get_line_dash()
        -> ext::vector<double>
{
    // return the dash list (TODO : why isn't this a property?)
    return m_dash_list;
}


template <typename T>
auto html::canvas::mixins::canvas_path_drawing_styles<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_path_drawing_styles<T>>{isolate}
            .template inherit<dom_object>()
            .template function("getLineDash", &canvas_path_drawing_styles<T>::get_line_dash)
            .template function("setLineDash", &canvas_path_drawing_styles<T>::set_line_dash)
            .template var("lineDashOffset", &canvas_path_drawing_styles<T>::line_dash_offset)
            .template var("lineWidth", &canvas_path_drawing_styles<T>::line_width)
            .template var("miterLimit", &canvas_path_drawing_styles<T>::miter_limit)
            .template var("lineCap", &canvas_path_drawing_styles<T>::line_cap)
            .template var("lineJoin", &canvas_path_drawing_styles<T>::line_join)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_path_drawing_styles<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_path_drawing_styles<html::canvas::offscreen_canvas_rendering_context_2d>;
