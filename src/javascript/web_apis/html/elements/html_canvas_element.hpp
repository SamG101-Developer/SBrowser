#pragma once
#ifndef SBROWSER_HTML_CANVAS_ELEMENT_HPP
#define SBROWSER_HTML_CANVAS_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace file::objects {class blob;}
namespace html::elements {class html_canvas_element;}
namespace html::canvasing {class canvas_rendering_context_2d;}
namespace html::canvasing {class image_bitmap_rendering_context;}
namespace html::canvasing {class offscreen_canvas;}
namespace html::images {class image_bitmap;}


class html::elements::html_canvas_element : public html::elements::html_element
{
public aliases:
    using blob_callback_t = std::function<void(file::objects::blob*)>;
    using rendering_context_t = std::variant<canvasing::canvas_rendering_context_2d, canvasing::image_bitmap_rendering_context>;

public enums:
    enum class context_mode {PLACE_HOLDER, _2D, BITMAPRENDERER, WEBGL, WEBGL2, WEBGPU, NONE};

public constructors:
    html_canvas_element();

public methods:
    new_obj auto get_context(const ext::string& context_id, ext::any* options = nullptr) -> rendering_context_t;
    new_obj auto transfer_control_to_offscreen() -> canvasing::offscreen_canvas;
    auto to_data_url(const ext::string& type = "image/png", ext::any* quality = nullptr) -> ext::string;
    auto to_blob(blob_callback_t&& callback, const ext::string& type = "image/png", ext::any* quality = nullptr);

public properties:
    ext::html_property<unsigned long, _T> width;
    ext::html_property<unsigned long, _T> height;

public internal_properties:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

protected internal_properties:
    context_mode m_context_mode;
    images::image_bitmap* m_bitmap = nullptr;
};


#endif //SBROWSER_HTML_CANVAS_ELEMENT_HPP
