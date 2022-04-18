#pragma once
#ifndef SBROWSER_PAINTER_HPP
#define SBROWSER_PAINTER_HPP

#include <variant>

#include <ext/decorators.hpp>
#include <ext/string.hpp>

#include <QtGui/QPainter>

namespace render::painting {class painter;}
namespace render::painting {class painter_path;}
namespace html::canvas {class canvas_rendering_context_2d;}
namespace html::canvas::paint {class canvas_gradient;}
namespace html::canvas::paint {class canvas_pattern;}


class render::painting::painter
{
public aliases:
    using paint_style_t = std::variant<ext::string, html::canvas::paint::canvas_gradient*, html::canvas::paint::canvas_pattern*>;

public js_methods:
    auto fill_path(painter_path& path, const paint_style_t& fill_style) -> void;
    auto stroke_path(painter_path& path, const paint_style_t& stroke_style) -> void;

    auto fill_rect(double x, double y, double w, double h, const paint_style_t& fill_style) -> void;

public operators:
    explicit operator QPainter() const;

private cpp_properties:
    QPainter m_painter;
};


#endif //SBROWSER_PAINTER_HPP
