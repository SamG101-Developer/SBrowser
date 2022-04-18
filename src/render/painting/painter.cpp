#include "painter.hpp"

#include <html/canvas/paint/canvas_gradient.hpp>
#include <html/canvas/paint/canvas_pattern.hpp>

#include <QtCore/QRectF>


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
