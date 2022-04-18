#pragma once
#ifndef SBROWSER_PEN_HPP
#define SBROWSER_PEN_HPP

#include <ext/decorators.hpp>
#include <ext/map.hpp>
#include <ext/string.hpp>

#include <QtGui/QPen>

namespace render::painting {class pen;}


class render::painting::pen
{
public constructors:
    pen();

public cpp_methods:
    auto set_pen_width(double width) -> void;
    auto set_miter_limit(double limit) -> void;
    auto set_cap_style(const ext::string& cap_style) -> void;
    auto set_join_style(const ext::string& join_style) -> void;

public operators:
    operator QPen() const;

public cpp_properties:
    ext::map<ext::string, Qt::PenCapStyle> translate_cap_style;
    ext::map<ext::string, Qt::PenJoinStyle> translate_join_style;

private cpp_properties:
    QPen m_pen;
};


#endif //SBROWSER_PEN_HPP
