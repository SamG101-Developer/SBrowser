#pragma once
#ifndef SBROWSER_CANVAS_FILL_STROKE_STYLES_HPP
#define SBROWSER_CANVAS_FILL_STROKE_STYLES_HPP

#include <variant>
#include <dom_object.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_fill_stroke_styles;}
namespace html::canvas::mixins {template <typename T> class canvas_rect;}
namespace html::canvas::mixins {template <typename T> class canvas_text;}
namespace html::canvas::paint {class canvas_gradient;}
namespace html::canvas::paint {class canvas_pattern;}
namespace html::elements {class html_element;}


template <typename T>
class html::canvas::mixins::canvas_fill_stroke_styles : public virtual dom_object
{
public friends:
    using paint_style_t = std::variant<ext::string, paint::canvas_gradient*, paint::canvas_pattern*>;

public aliases:
    template <typename U> friend class canvas_rect;
    template <typename U> friend class canvas_text;

public constructors:
    canvas_fill_stroke_styles();

public js_properties:
    ext::html_property<paint_style_t, _F> fill_style;
    ext::html_property<paint_style_t, _F> stroke_style;

public js_methods:
    new_obj auto create_linear_gradient(double x0, double y0, double x1, double y1) -> paint::canvas_gradient;
    new_obj auto create_radial_gradient(double x0, double y0, double r0, double x1, double y1, double r1) -> paint::canvas_gradient;
    new_obj auto create_conic_gradient(double start_angle, double x, double y) -> paint::canvas_gradient;
    new_obj auto create_pattern(elements::html_element* image) -> paint::canvas_pattern;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_FILL_STROKE_STYLES_HPP
