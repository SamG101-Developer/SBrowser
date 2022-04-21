#include "offscreen_canvas_rendering_context_2d.hpp"

#include <dom/helpers/mutation_observers.hpp>


auto html::canvas::offscreen_canvas_rendering_context_2d::commit() -> void
{
    if (not canvas->m_placeholder_canvas) return;

    auto image = decltype(m_output_bitmap)(m_output_bitmap); // TODO : does this copy?
    dom::helpers::mutation_observers::queue_element_task(canvas->m_placeholder_canvas, [image] {canvas->m_placeholder_canvas->m_output_bitmap = image;});
}
