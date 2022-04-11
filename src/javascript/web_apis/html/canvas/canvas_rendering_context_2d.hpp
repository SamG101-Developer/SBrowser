#pragma once
#ifndef SBROWSER_CANVAS_RENDERING_CONTEXT_2D_HPP
#define SBROWSER_CANVAS_RENDERING_CONTEXT_2D_HPP

#include <ext/map.hpp>

#include <html/canvasing/abstract_rendering_context.hpp>
#include <html/canvasing/mixins/canvas_state.hpp>
#include <html/canvasing/mixins/canvas_transform.hpp>
#include <html/canvasing/mixins/canvas_compositing.hpp>
#include <html/canvasing/mixins/canvas_image_smoothing.hpp>
#include <html/canvasing/mixins/canvas_fill_stroke_styles.hpp>
#include <html/canvasing/mixins/canvas_shadow_styles.hpp>
#include <html/canvasing/mixins/canvas_filters.hpp>
#include <html/canvasing/mixins/canvas_rect.hpp>
#include <html/canvasing/mixins/canvas_draw_path.hpp>
#include <html/canvasing/mixins/canvas_user_interface.hpp>
#include <html/canvasing/mixins/canvas_text.hpp>
#include <html/canvasing/mixins/canvas_draw_image.hpp>
#include <html/canvasing/mixins/canvas_image_data.hpp>
#include <html/canvasing/mixins/canvas_path_drawing_styles.hpp>
#include <html/canvasing/mixins/canvas_text_drawing_styles.hpp>
#include <html/canvasing/mixins/canvas_path.hpp>

namespace html::canvasing {class canvas_rendering_context_2d;}
namespace html::elements {class html_canvas_element;}


class html::canvasing::canvas_rendering_context_2d
        : public abstract_rendering_context
        , public mixins::canvas_state<canvas_rendering_context_2d>
        , public mixins::canvas_transform<canvas_rendering_context_2d>
        , public mixins::canvas_compositing<canvas_rendering_context_2d>
        , public mixins::canvas_image_smoothing<canvas_rendering_context_2d>
        , public mixins::canvas_fill_stroke_styles<canvas_rendering_context_2d>
{
public js_methods:
    ext::string_any_map get_context_attributes() const;

public js_properties:
    ext::dom_property<elements::html_canvas_element*, _F> canvas;
};


#endif //SBROWSER_CANVAS_RENDERING_CONTEXT_2D_HPP
