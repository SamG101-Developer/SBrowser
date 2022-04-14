#pragma once
#ifndef SBROWSER_CANVAS_PATH_HPP
#define SBROWSER_CANVAS_PATH_HPP

#include <dom_object.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_path;}


template<typename T>
class html::canvas::mixins::canvas_path : public virtual dom_object
{
public js_methods:
    auto close_path() -> void;
    auto move_to(double x, double y) -> void;
    auto line_to(double x, double y) -> void;
    auto quadratic_curve_to(double cpx, double cpy, double x, double y) -> void;
    auto bezier_curve_to(double cp1x, double cp1y, double cp2x, double cp2y, double x, double y) -> void;
    auto arc_to(double x1, double y1, double x2, double y2, double radius) -> void;
    auto rect(double x, double y, double w, double h) -> void;
    auto round_rect(double x, double y, double w, double h, const ext::vector<int>& radii) -> void;
    auto arc(double x, double y, double radius, double start_angle, double end_angle, bool counterclockwise = false) -> void;
    auto ellpise(double x, double y, double radiux_x, double radius_y, double rotation, double, double start_angle, double end_angle, bool counterclockwise = false) -> void;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_PATH_HPP
