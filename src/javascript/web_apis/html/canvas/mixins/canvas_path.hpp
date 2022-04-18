#pragma once
#ifndef SBROWSER_CANVAS_PATH_HPP
#define SBROWSER_CANVAS_PATH_HPP

#include <dom_object.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_path;}
namespace html::canvas::mixins {template <typename T> class canvas_draw_path;}
namespace html::internal {struct point;}
namespace html::internal {struct subpath;}


template<typename T>
class html::canvas::mixins::canvas_path : public virtual dom_object
{
public aliases:
    using point_t = html::internal::point;
    using sub_path_t = html::internal::subpath;
    using path_t = ext::vector<sub_path_t>;

public friends:
    template <typename U> friend class canvas_draw_path;

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
    auto ellipse(double x, double y, double radius_x, double radius_y, double rotation, double start_angle, double end_angle, bool counterclockwise = false) -> void;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected cpp_properties:
    path_t m_path;
    bool m_need_new_subpath;
};


#endif //SBROWSER_CANVAS_PATH_HPP
