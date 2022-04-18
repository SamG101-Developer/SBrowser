#pragma once
#ifndef SBROWSER_CANVAS_PATH_DRAWING_STYLES_HPP
#define SBROWSER_CANVAS_PATH_DRAWING_STYLES_HPP

#include <dom_object.hpp>
#include <render/painting/pen.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_path_drawing_styles;}


template<typename T>
class html::canvas::mixins::canvas_path_drawing_styles : public virtual dom_object
{
public constructors:
    canvas_path_drawing_styles();

public js_methods:
    new_obj auto get_line_dash() -> ext::vector<double>;
    auto set_line_dash(const ext::vector<double>& segments) -> void;

public js_properties:
    ext::html_property<double, _F> line_dash_offset;
    ext::html_property<double, _F> line_width;
    ext::html_property<double, _F> miter_limit;
    ext::html_property<ext::string, _F> line_cap;
    ext::html_property<ext::string, _F> line_join;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    render::painting::pen m_pen;
    ext::vector<double> m_dash_list;
};


#endif //SBROWSER_CANVAS_PATH_DRAWING_STYLES_HPP
