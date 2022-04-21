#pragma once
#ifndef SBROWSER_ABSTRACT_RENDERING_CONTEXT_HPP
#define SBROWSER_ABSTRACT_RENDERING_CONTEXT_HPP

#include <dom_object.hpp>

#include <render/painting/painter.hpp>

namespace html::canvas {class abstract_rendering_context;}
namespace html::elements {class html_canvas_element;}
namespace html::helpers {struct canvas_internals;}
namespace html::canvas::mixins {template <typename T> class canvas_rect;}
namespace html::canvas::mixins {template <typename T> class canvas_text;}
namespace html::canvas::mixins {template <typename T> class canvas_image_data;}
namespace html::canvas::mixins {template <typename T> class canvas_compositing;}
namespace html::canvas::mixins {template <typename T> class canvas_shadow_styles;}
namespace html::canvas::mixins {template <typename T> class draw_path;}
namespace html::canvas::paint {class image_bitmap;}


class html::canvas::abstract_rendering_context : public virtual dom_object
{
public friends:
    friend struct helpers::canvas_internals;
    template <typename T> friend class mixins::canvas_rect;
    template <typename T> friend class mixins::canvas_text;
    template <typename T> friend class mixins::canvas_image_data;
    template <typename T> friend class mixins::canvas_compositing;
    template <typename T> friend class mixins::canvas_shadow_styles;
    template <typename T> friend class mixins::draw_path;

public js_properties:
    ext::dom_property<elements::html_canvas_element*, _F> canvas;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

protected cpp_properties:
    struct {bool m_origin_clean_flag = true; paint::image_bitmap* bitmap;} m_output_bitmap;
    ext::string m_color_space;
    render::painting::painter m_painter;
};


#endif //SBROWSER_ABSTRACT_RENDERING_CONTEXT_HPP
