#pragma once
#ifndef SBROWSER_CANVAS_DRAW_PATH_HPP
#define SBROWSER_CANVAS_DRAW_PATH_HPP

#include <dom_object.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_draw_path;}
namespace html::canvas::paint {class path_2d;}


template <typename T>
class html::canvas::mixins::canvas_draw_path : public virtual dom_object
{
public js_methods:
    auto begin_path() -> void;

    auto fill(const ext::string& fill_rule = "nonzero") -> void;
    auto fill(paint::path_2d* path, const ext::string& fill_rule = "nonzero") -> void;
    auto stroke() -> void;
    auto stroke(paint::path_2d* path) -> void;
    auto clip(const ext::string& fill_rule = "nonzero") -> void;
    auto clip(paint::path_2d* path, const ext::string& fill_rule = "nonzero") -> void;

    auto is_point_in_path(double x, double y, const ext::string& fill_rule = "nonzero");
    auto is_point_in_path(paint::path_2d* path, double x, double y, const ext::string& fill_rule = "nonzero");
    auto is_point_in_stroke(double x, double y);
    auto is_point_in_stroke(paint::path_2d* path, double x, double y);

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_DRAW_PATH_HPP
