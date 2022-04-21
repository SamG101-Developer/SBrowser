#pragma once
#ifndef SBROWSER_OFFSCREEN_offscreen_canvas_rendering_context_2d_HPP
#define SBROWSER_OFFSCREEN_offscreen_canvas_rendering_context_2d_HPP

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
#include <html/canvas/mixins/canvas_text.hpp>
#include <html/canvas/mixins/canvas_draw_image.hpp>
#include <html/canvas/mixins/canvas_image_data.hpp>
#include <html/canvas/mixins/canvas_path_drawing_styles.hpp>
#include <html/canvas/mixins/canvas_text_drawing_styles.hpp>
#include <html/canvas/mixins/canvas_path.hpp>

namespace html::canvas {class offscreen_canvas_rendering_context_2d;}


class html::canvas::offscreen_canvas_rendering_context_2d
        : public abstract_rendering_context
        , public mixins::canvas_state<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_transform<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_compositing<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_image_smoothing<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_fill_stroke_styles<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_shadow_styles<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_filters<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_rect<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_draw_path<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_text<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_draw_image<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_image_data<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_path_drawing_styles<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_text_drawing_styles<offscreen_canvas_rendering_context_2d>
        , public mixins::canvas_path<offscreen_canvas_rendering_context_2d>
{
public js_methods:
    auto commit() -> void;

public js_properties:
    // TODO : offscreen canvas

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_OFFSCREEN_offscreen_canvas_rendering_context_2d_HPP
