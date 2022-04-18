#pragma once
#ifndef SBROWSER_CANVAS_GRADIENT_HPP
#define SBROWSER_CANVAS_GRADIENT_HPP

#include <dom_object.hpp>

#include <QtGui/QGradient>

namespace html::canvas::paint {class canvas_gradient;}
namespace html::canvas::mixins {template <typename T> class canvas_fill_stroke_styles;}


class html::canvas::paint::canvas_gradient : public virtual dom_object
{
public friends:
    template <typename T> friend class mixins::canvas_fill_stroke_styles;

public js_methods:
    auto add_color_stop(double offset, const ext::string& color) -> void;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

public operators:
    explicit operator QGradient() const;

private cpp_properties:
    QGradient* m_gradient;
};


#endif //SBROWSER_CANVAS_GRADIENT_HPP
