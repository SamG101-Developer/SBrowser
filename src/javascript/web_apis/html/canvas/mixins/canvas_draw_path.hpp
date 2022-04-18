#pragma once
#ifndef SBROWSER_CANVAS_DRAW_PATH_HPP
#define SBROWSER_CANVAS_DRAW_PATH_HPP

#include <dom_object.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_draw_path;}
namespace html::canvas::mixins {template <typename T> class canvas_path;}
namespace html::canvas::paint {class path_2d;}


template <typename T>
class html::canvas::mixins::canvas_draw_path : public virtual dom_object
{
public aliases:
    using sub_path_t = typename canvas_path<T>::sub_path_t;
    using path_t     = typename canvas_path<T>::path_t;

public js_methods:
    auto begin_path() -> void;

    auto fill(const ext::string& fill_rule = "nonzero") -> void;
    auto fill(paint::path_2d* path, const ext::string& fill_rule = "nonzero") -> void;
    auto stroke() -> void;
    auto stroke(paint::path_2d* path) -> void;
    auto clip(const ext::string& fill_rule = "nonzero") -> void;
    auto clip(paint::path_2d* path, const ext::string& fill_rule = "nonzero") -> void;

    auto is_point_in_path(double x, double y, const ext::string& fill_rule = "nonzero") -> bool;
    auto is_point_in_path(paint::path_2d* path, double x, double y, const ext::string& fill_rule = "nonzero") -> bool;
    auto is_point_in_stroke(double x, double y) -> bool;
    auto is_point_in_stroke(paint::path_2d* path, double x, double y) -> bool;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    path_t m_current_default_path;
};


#endif //SBROWSER_CANVAS_DRAW_PATH_HPP
