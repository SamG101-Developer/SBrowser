#pragma once
#ifndef SBROWSER_PAINTER_PATH_HPP
#define SBROWSER_PAINTER_PATH_HPP

#include <ext/decorators.hpp>

#include <QtCore/QPointF>
#include <QtCore/QSizeF>
#include <QtGui/QFont>
#include <QtGui/QPainterPath>

namespace render::painting {class painter_path;}


class render::painting::painter_path
{
public cpp_methods:
    auto line_to(const QPointF& to_point) -> void;
    auto quad_to(const QPointF& control_point, const QPointF& to_point) -> void;
    auto cubic_to(const QPointF& control_point_1, const QPointF& control_point_2, const QPointF to_point) -> void;

    auto add_rect(const QPointF& position, const QSizeF& size) -> void;
    auto add_text(const QPointF& position, const QFont& font, const ext::string& text = "");
};


#endif //SBROWSER_PAINTER_PATH_HPP
