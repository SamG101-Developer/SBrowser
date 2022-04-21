#pragma once
#ifndef SBROWSER_PAINTER_HPP
#define SBROWSER_PAINTER_HPP

#include <variant>

#include <ext/decorators.hpp>
#include <ext/string.hpp>

#include <QtWidgets/QGraphicsDropShadowEffect>
#include <QtGui/QImage>
#include <QtGui/QPainter>

namespace render::painting {class image;}
namespace render::painting {class painter;}
namespace render::painting {class painter_path;}
namespace html::canvas {class canvas_rendering_context_2d;}
namespace html::canvas::paint {class canvas_gradient;}
namespace html::canvas::paint {class canvas_pattern;}


class render::painting::painter
{
public aliases:
    using paint_style_t = std::variant<ext::string, html::canvas::paint::canvas_gradient*, html::canvas::paint::canvas_pattern*>;

public constructors:
    painter();

public js_methods:
    auto fill_path(painter_path& path, const paint_style_t& fill_style) -> void;
    auto fill_rect(const QPointF& position, const QSizeF& size, const paint_style_t& fill_style) -> void;
    auto fill_image(const QPointF& position, const QSizeF& size, const render::painting::image& image) -> void;

    auto stroke_path(painter_path& path, const paint_style_t& stroke_style) -> void;

    auto erase_rect(const QPointF& poition, const QSizeF& size) -> void;

    auto set_image_smoothing_enabled(bool image_smoothing_enabled) -> void;
    auto set_image_smoothing_quality(Qt::TransformationMode image_smoothing_quality) -> void;

    auto shadow_effect() -> QGraphicsDropShadowEffect;

public operators:
    explicit operator QPainter() const;

public cpp_properties:
    ext::map<ext::string, QPainter::CompositionMode> translate_composition_mode;
    ext::map<ext::string, Qt::TransformationMode   > translate_image_smoothing_quality;

private cpp_properties:
    QPainter m_painter;

    bool m_image_smoothing_enabled = false;
    Qt::TransformationMode m_image_smoothing_quality;

    QGraphicsDropShadowEffect m_shadow_effect;
};


#endif //SBROWSER_PAINTER_HPP
