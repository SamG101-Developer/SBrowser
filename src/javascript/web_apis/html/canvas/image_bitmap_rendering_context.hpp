#pragma once
#ifndef SBROWSER_IMAGE_BITMAP_RENDERING_CONTEXT_HPP
#define SBROWSER_IMAGE_BITMAP_RENDERING_CONTEXT_HPP

#include <html/canvas/abstract_rendering_context.hpp>

namespace html::canvas {class image_bitmap_rendering_context;}
namespace html::canvas::paint {class image_bitmap;}


class html::canvas::image_bitmap_rendering_context : public abstract_rendering_context
{
public js_methods:
    auto transfer_from_image_bitmap(paint::image_bitmap* bitmap = nullptr) -> void;

public js_properties:
    ext::html_property<elements::html_canvas_element*, false> canvas;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private js_slots:
    bool s_detached = false;

private cpp_properties:
    bool m_bitmap_mode = false;
    bool m_alpha_flag  = false;
};


#endif //SBROWSER_IMAGE_BITMAP_RENDERING_CONTEXT_HPP
