#include "painter.hpp"

#include <html/canvas/paint/canvas_gradient.hpp>
#include <html/canvas/paint/canvas_pattern.hpp>

#include <render/painting/image.hpp>

#include <QtCore/QRectF>


render::painting::painter::painter()
{
    translate_composition_mode =
    {
            {"clear", QPainter::CompositionMode::CompositionMode_Clear},

            {"copy", QPainter::CompositionMode::CompositionMode_Source},
            {"source-over", QPainter::CompositionMode::CompositionMode_SourceOver},
            {"source-in", QPainter::CompositionMode::CompositionMode_SourceIn},
            {"source-out", QPainter::CompositionMode::CompositionMode_SourceOut},
            {"source-atop", QPainter::CompositionMode::CompositionMode_SourceAtop},

            {"destination", QPainter::CompositionMode::CompositionMode_Destination},
            {"destination-over", QPainter::CompositionMode::CompositionMode_DestinationOver},
            {"destination-in", QPainter::CompositionMode::CompositionMode_DestinationIn},
            {"destination-out", QPainter::CompositionMode::CompositionMode_DestinationOut},
            {"destination-atop", QPainter::CompositionMode::CompositionMode_DestinationAtop},

            {"xor", QPainter::CompositionMode::CompositionMode_Xor},
//            {"lighter", QPainter::CompositionMode::CompositionMode_Lighten},
//            {"plus-lighter", QPainter::CompositionMode::CompositionMode_HardLight},
//            {"plus-darker", QPainter::CompositionMode::CompositionMode_SoftLight},

            {"normal", QPainter::CompositionMode::CompositionMode_Source},
            {"multiply", QPainter::CompositionMode::CompositionMode_Multiply},
            {"screen", QPainter::CompositionMode::CompositionMode_Screen},
            {"multiply", QPainter::CompositionMode::CompositionMode_Multiply},
            {"lighten", QPainter::CompositionMode::CompositionMode_Lighten},
            {"darken", QPainter::CompositionMode::CompositionMode_Darken},
            {"color-dodge", QPainter::CompositionMode::CompositionMode_ColorDodge},
            {"color-burn", QPainter::CompositionMode::CompositionMode_ColorBurn},
//            {"saturation", QPainter::CompositionMode::CompositionMode_},
//            {"color", QPainter::CompositionMode::CompositionMode_},
//            {"luminosity", QPainter::CompositionMode::CompositionMode_},
    };

    translate_image_smoothing_quality =
    {
            {"low", Qt::TransformationMode::FastTransformation},
            {"medium", Qt::TransformationMode::FastTransformation}, // TODO : currently ame as low
            {"high", Qt::TransformationMode::SmoothTransformation},
    };
}


auto render::painting::painter::fill_rect(
        const double x,
        const double y,
        const double w,
        const double h,
        const paint_style_t& fill_style)
        -> void
{
    if (std::holds_alternative<html::canvas::paint::canvas_gradient*>(fill_style))
        m_painter.fillRect(QRectF{x, y, w, h}, (QGradient)*std::get<html::canvas::paint::canvas_gradient*>(fill_style));

    else if (std::holds_alternative<html::canvas::paint::canvas_pattern*>(fill_style))
        m_painter.drawImage(QRectF{x, y, w, h}, (QImage)*std::get<html::canvas::paint::canvas_pattern*>(fill_style));

    else if (std::holds_alternative<ext::string>(fill_style))
        m_painter.fillRect(QRectF{x, y, w, h}, QColor{std::get<ext::string>(fill_style)});
}


auto render::painting::painter::fill_image(
        const double x,
        const double y,
        const double w,
        const double h,
        const render::painting::image& image)
        -> void
{
    m_painter.drawImage(QRectF{x, y, w, h}, static_cast<QImage>(image).scaled(
            static_cast<int>(w),
            static_cast<int>(h),
            Qt::AspectRatioMode::KeepAspectRatio,
            m_image_smoothing_enabled ? m_image_smoothing_quality : Qt::TransformationMode::FastTransformation));
}
