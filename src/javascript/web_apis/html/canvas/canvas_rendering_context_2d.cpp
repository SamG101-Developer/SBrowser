#include "canvas_rendering_context_2d.hpp"


auto html::canvas::canvas_rendering_context_2d::get_context_attributes() const
        -> const ext::string_any_map_t
{
    // return the context attributes of the render context
    return
    {
        {"alpha", m_alpha},
        {"desynchronized", m_desynchronized},
        {"colorSpace", m_color_space},
        {"willReadFrequently", m_will_read_frequently}
    };
}
