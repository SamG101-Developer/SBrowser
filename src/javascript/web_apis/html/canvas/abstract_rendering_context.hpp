#pragma once
#ifndef SBROWSER_ABSTRACT_RENDERING_CONTEXT_HPP
#define SBROWSER_ABSTRACT_RENDERING_CONTEXT_HPP

#include <dom_object.hpp>
#include <QtGui/QPainter>

namespace html::canvas {class abstract_rendering_context;}
namespace html::elements {class html_canvas_element;}


class html::canvas::abstract_rendering_context : public virtual dom_object
{
public js_properties:
    ext::dom_property<elements::html_canvas_element*, _F> canvas;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private cpp_properties:
    QPainter m_painter;
};


#endif //SBROWSER_ABSTRACT_RENDERING_CONTEXT_HPP
