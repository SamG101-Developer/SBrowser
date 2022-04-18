#include "pen.hpp"


render::painting::pen::pen()
{
    translate_cap_style =
    {
            {"butt", Qt::FlatCap},
            {"round", Qt::RoundCap},
            {"square", Qt::SquareCap}
    };

    translate_join_style =
    {
            {"round", Qt::RoundJoin},
            {"bevel", Qt::BevelJoin},
            {"miter", Qt::MiterJoin}
    };
}


auto render::painting::pen::set_pen_width(
        double width)
        -> void
{
    // set the pen width
    m_pen.setWidth(static_cast<int>(width));
}


auto render::painting::pen::set_miter_limit(
        double limit)
        -> void
{
    // set the miter limit
    m_pen.setMiterLimit(static_cast<qreal>(limit));
}


auto render::painting::pen::set_cap_style(
        const ext::string& cap_style)
        -> void
{
    // set the cap style
    m_pen.setCapStyle(translate_cap_style.at(cap_style));
}


auto render::painting::pen::set_join_style(
        const ext::string& join_style)
        -> void
{
    // set the join style
    m_pen.setJoinStyle(translate_join_style.at(join_style));
}


render::painting::pen::operator QPen() const
{
    // cast into a QPen
    return m_pen;
}
