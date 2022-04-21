#pragma once
#ifndef SBROWSER_CANVAS_RENDERING_CONTEXT_2D_HPP
#define SBROWSER_CANVAS_RENDERING_CONTEXT_2D_HPP

#include <ext/map.hpp>

#include <html/canvas/abstract_rendering_context.hpp>
#include <html/canvas/mixins/canvas_state.hpp>
#include <html/canvas/mixins/canvas_transform.hpp>
#include <html/canvas/mixins/canvas_compositing.hpp>
#include <html/canvas/mixins/canvas_image_smoothing.hpp>
#include <html/canvas/mixins/canvas_fill_stroke_styles.hpp>
#include <html/canvas/mixins/canvas_shadow_styles.hpp>
#include <html/canvas/mixins/canvas_filters.hpp>
#include <html/canvas/mixins/canvas_rect.hpp>
#include <html/canvas/mixins/canvas_draw_path.hpp>
#include <html/canvas/mixins/canvas_user_interface.hpp>
#include <html/canvas/mixins/canvas_text.hpp>
#include <html/canvas/mixins/canvas_draw_image.hpp>
#include <html/canvas/mixins/canvas_image_data.hpp>
#include <html/canvas/mixins/canvas_path_drawing_styles.hpp>
#include <html/canvas/mixins/canvas_text_drawing_styles.hpp>
#include <html/canvas/mixins/canvas_path.hpp>

namespace html::canvas {class canvas_rendering_context_2d;}
namespace html::helpers {struct canvas_internals;}


class html::canvas::canvas_rendering_context_2d
        : public abstract_rendering_context
        , public mixins::canvas_state<canvas_rendering_context_2d>
        , public mixins::canvas_transform<canvas_rendering_context_2d>
        , public mixins::canvas_compositing<canvas_rendering_context_2d>
        , public mixins::canvas_image_smoothing<canvas_rendering_context_2d>
        , public mixins::canvas_fill_stroke_styles<canvas_rendering_context_2d>
        , public mixins::canvas_shadow_styles<canvas_rendering_context_2d>
        , public mixins::canvas_filters<canvas_rendering_context_2d>
        , public mixins::canvas_rect<canvas_rendering_context_2d>
        , public mixins::canvas_draw_path<canvas_rendering_context_2d>
        , public mixins::canvas_user_interface<canvas_rendering_context_2d>
        , public mixins::canvas_text<canvas_rendering_context_2d>
        , public mixins::canvas_draw_image<canvas_rendering_context_2d>
        , public mixins::canvas_image_data<canvas_rendering_context_2d>
        , public mixins::canvas_path_drawing_styles<canvas_rendering_context_2d>
        , public mixins::canvas_text_drawing_styles<canvas_rendering_context_2d>
        , public mixins::canvas_path<canvas_rendering_context_2d>
{
public js_methods:
    auto get_context_attributes() const -> const ext::string_any_map;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    bool m_alpha;
    bool m_desynchronized;
    bool m_will_read_frequently;
};


#endif //SBROWSER_CANVAS_RENDERING_CONTEXT_2D_HPP
