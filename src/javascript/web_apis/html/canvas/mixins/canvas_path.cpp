#include "canvas_path.hpp"

#include <ext/infinity.hpp>

#include <dom/helpers/exceptions.hpp>

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/offscreen_canvas_rendering_context_2d.hpp>

#include <html/canvas/paint/path_2d.hpp>
#include <html/helpers/canvas_internals.hpp>


template <typename T>
auto html::canvas::mixins::canvas_path<T>::close_path()
        -> void
{
    // return if the path is empty
    if (m_path.empty())
        return;

    // set the previous subpath to being closed
    m_path.back().is_subpath_closed = true;

    // add a new subpath whose first point is the same as the previous subpath's first point
    ext::vector<point_t> previous_subpath = m_path.back().segments;
    m_path.append(sub_path_t{previous_subpath.front()});

}


template <typename T>
auto html::canvas::mixins::canvas_path<T>::move_to(
        double x,
        double y)
        -> void
{
    if (ext::infinity<double>::is_inf_or_nan(x, y)) return;

    // append a new subpath containing the point {x, y}
    point_t point {.x=x, .y=y};
    sub_path_t sub_path{.segments={point}, .is_subpath_closed = false};
    m_path.append(sub_path);
}


template <typename T>
auto html::canvas::mixins::canvas_path<T>::line_to(
        const double x,
        const double y) -> void
{
    if (ext::infinity<double>::is_inf_or_nan(x, y)) return;

    // create a mock point, and branch on if the path is empty or not
    point_t mock_point{.x=x, .y=y};
    if (m_path.empty())

        // if the path is empty, ensure there is a subpath for the mock point
        helpers::canvas_internals::ensure_there_is_subpath(mock_point, this);
    else
    {
        // otherwise, append a new mock point to the last subpath, and paint the line
        m_path.back().qt_painter_subpath.line_to(mock_point);
        m_path.back().segments.append(mock_point);
    }
}


template <typename T>
auto html::canvas::mixins::canvas_path<T>::quadratic_curve_to(
        const double cpx,
        const double cpy,
        const double x,
        const double y)
        -> void
{
    if (ext::infinity<double>::is_inf_or_nan(cpx, cpy, x, y)) return;

    // create mock points for the end point and control point
    point_t mock_control_point{.x=cpx, .y=cpy};
    point_t mock_point{.x=x, .y=y};

    // ensure there is a subpath for the control point, append the point, and paint the curve
    helpers::canvas_internals::ensure_there_is_subpath(mock_control_point, this);
    m_path.back().qt_painter_subpath.quad_to(mock_control_point, mock_point);
    m_path.back().segments.append(mock_point);
}


template<typename T>
auto html::canvas::mixins::canvas_path<T>::bezier_curve_to(
        const double cp1x,
        const double cp1y,
        const double cp2x,
        const double cp2y,
        const double x,
        const double y)
        -> void
{
    if (ext::infinity<double>::is_inf_or_nan(cp1x, cp1y, cp2x, cp2y, x, y)) return;

    // create mock points for the end point and control points
    point_t mock_control_point_1{.x=cp1x, .y=cp1y};
    point_t mock_control_point_2{.x=cp2x, .y=cp2y};
    point_t mock_point{.x=x, .y=y};

    // ensure there is a subpath for the first control point, append the point, and paint the curve
    helpers::canvas_internals::ensure_there_is_subpath(mock_control_point_1, this);
    m_path.back().qt_painter_subpath.cubic_to(mock_control_point_1, mock_control_point_2, mock_point);
    m_path.back().segments.append(mock_point);
}


template<typename T>
auto html::canvas::mixins::canvas_path<T>::arc_to(
        const double x1,
        const double y1,
        const double x2,
        const double y2,
        const double radius)
        -> void
{
    if (ext::infinity<double>::is_inf_or_nan(x1, y1, x2, y2, radius)) return;

    // if the radius is negative, throw an index size error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "radius must be positive",
            [radius] {return radius < 0;});

    // create mock points for the two arc points
    point_t mock_point_1{.x=x1, .y=y1};
    point_t mock_point_2{.x=x2, .y=y2};
    point_t mock_point_0 = m_path.back().segments.back(); // TODO : transform

    // if the previous point is the same as the first point, or the control points are the same, append the first mock
    // point, and paint the line between the previous point and the first mock point
    if (mock_point_0 == mock_point_1 or mock_point_1 == mock_point_2 or radius == 0)
    {
        m_path.back().qt_painter_subpath.line_to(mock_point_1);
        m_path.back().segments.append(mock_point_1);
    }

    // if the previous point and the two arc points are collinear, append the first mock point, and paint the line
    // between the previous point and the first mock point
    else if (helpers::canvas_internals::are_collinear(mock_point_0, mock_point_1, mock_point_2))
    {
        m_path.back().qt_painter_subpath.line_to(mock_point_1);
        m_path.back().segments.append(mock_point_1);
    }

    else
    {
        // TODO
    }
}


template <typename T>
auto html::canvas::mixins::canvas_path<T>::rect(
        const double x,
        const double y,
        const double w,
        const double h)
        -> void
{
    if (ext::infinity<double>::is_inf_or_nan(x, y, w, h)) return;

    // create the rectangle sub path by adding the 4 corners, and mark it as a closed path
    sub_path_t rectangle
    {
        .segments={point_t{x, y}, point_t{x + w, y}, point_t{x + w, y + h}, point_t{x, y + h}},
        .is_subpath_closed = true
    };

    // create the mock point, append the subpath, and paint the rectangle
    point_t mock_point{.x=x, .y=y};
    m_path.append(rectangle);
    m_path.back().qt_painter_subpath.add_rect(mock_point, {w, h});

    // add it to a new subpath
    m_path.append(sub_path_t{mock_point});
}


template <typename T>
auto html::canvas::mixins::canvas_path<T>::arc(
        const double x,
        const double y,
        const double radius,
        const double start_angle,
        const double end_angle,
        const bool counterclockwise)
        -> void
{
    // create an ellipse with the both radii equal, and the rotation set to 0
    ellipse(x, y, radius, radius, 0, start_angle, end_angle, true);
}


template <typename T>
auto
html::canvas::mixins::canvas_path<T>::ellipse(
        const double x,
        const double y,
        const double radius_x,
        const double radius_y,
        const double rotation,
        const double start_angle,
        const double end_angle,
        const bool counterclockwise)
        -> void
{
    if (ext::infinity<double>::is_inf_or_nan(x, y, radius_x, radius_y, rotation, start_angle, end_angle)) return;

    // if either radius is negative, throw an index size error
    dom::helpers::exceptions::throw_v8_exception<INDEX_SIZE_ERR>(
            "radiusX and radiusY must both be positive",
            [radius_x, radius_y] {return radius_x > 0 and radius_y > 0;});

    // create a mock point
    point_t mock_point{x, y};

    if (not m_path.empty())
        m_path.back().qt_painter_subpath.line_to(mock_point); // TODO : check

    // TODO
}


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
            .template function("ellipse", &canvas_path<T>::ellipse)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_path<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_path<html::canvas::offscreen_canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_path<html::canvas::paint::path_2d>;
