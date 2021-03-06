#include "html_canvas_element.hpp"

#include <dom/helpers/exceptions.hpp>

#include <html/helpers/custom_html_elements.hpp>
#include <html/helpers/canvas_internals.hpp>


html::elements::html_canvas_element::html_canvas_element()
        : html_element()
{
    // attach the qt functions
    width.template attach_qt_updater(&QWidget::setFixedWidth, m_rendered_widget);
    height.template attach_qt_updater(&QWidget::setFixedHeight, m_rendered_widget);

    // set the property's values
    width << 300UL;
    height << 150UL;
}


auto html::elements::html_canvas_element::get_context(
        const ext::string& context_id,
        ext::any* options)
        -> canvas::abstract_rendering_context*
{
    // convert the 'any' option into a type (int, string etc), to be usd in methods that are templates ie T mapped_options
    auto mapped_options = webidl::converting::convert_any(options);
    switch (m_context_mode)
    {
        case context_mode::_2D:
        {
            // create the 2d context with a predefined exception handler
            v8::TryCatch exception_handler{v8::Isolate::GetCurrent()};
            auto canvas_context = helpers::canvas_internals::_2d_context_creation_algorithm(this, mapped_options); // TODO : template context creation method

            // if there not was an exception, then return the created canvas context
            if (not exception_handler.HasCaught())
                return canvas_context;

            // otherwise, set the context mode to none and break
            m_context_mode = context_mode::NONE;
            break;
        }

        case context_mode::BITMAPRENDERER:
        {
            // create the image bitmap context, and return the created canvas context
            auto canvas_context = helpers::canvas_internals::image_bitmap_rendering_context_creation_algorithm(this, mapped_options);
            m_context_mode = context_mode::BITMAPRENDERER;
            return canvas_context;
        }

#if CONFIG_WEB_GL_SUPPORTED
        case context_mode::WEBGL:
        case context_mode::WEBGL2:
        {
            // TODO : webgl spec api
        }
#endif
#if CONFIG_WEB_GPU_SUPPORTED
        case context_mode::WEBGPU:
        {
            // TODO : webgpu spec api
        }
#endif
        default:
            return {};
    }
}


auto html::elements::html_canvas_element::transfer_control_to_offscreen()
        -> canvas::offscreen_canvas
{
    // if the canvas context is none, then throw an invalid state error
    dom::helpers::exceptions::throw_v8_exception<INVALID_STATE_ERR>(
            "canvas must have a context mode in order to transfer control to offscreen",
            [this] {return m_context_mode != context_mode::NONE;});

    // create a offscreen canvas with this canvas' size
    canvas::offscreen_canvas offscreen_canvas {width, height};
    offscreen_canvas.m_placeholder_canvas = this;

    // set this canvas context to a placeholder, and return the offscreen canvas
    m_context_mode = context_mode::PLACE_HOLDER;
    return offscreen_canvas;
}


auto html::elements::html_canvas_element::to_data_url(
        const ext::string& type,
        ext::any* quality)
        -> ext::string
{
    // if the image bitmap doesn't have a clean origin then throw a security error
    dom::helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "canvas' image bitmap must have a clean origin in order to be converted to a data url",
            [this] {return not m_bitmap->m_origin_clean_flag;});

    // return the empty data url if there are no pixels in the image bitmap
    if (m_bitmap->width == 0 or m_bitmap->height == 0)
        return "data:,";

    // get the file url by serializing the image, and return it with the data url prefix
    auto file = helpers::image_serialization::serialize_bitmap_as_file(type, quality);
    return "data:" + file;
}


auto html::elements::html_canvas_element::to_blob(
        blob_callback_t&& callback,
        const ext::string& type,
        ext::any* quality)
{
    // if the image bitmap doesn't have a clean origin then throw a security error
    dom::helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "canvas' image bitmap must have a clean origin in order to be converted to a data url",
            [this] {return not m_bitmap->m_origin_clean_flag;});

    file::objects::blob blob_object {};
    // TODO
}


auto html::elements::html_canvas_element::set_width(
        ulong val)
        -> void
{
    // if this canvas is a placeholder, then the width cannot be set
    dom::helpers::exceptions::throw_v8_exception<INVALID_STATE_ERR>(
            "canvas must not be placeholder in order to change its width",
            [this] {return m_context_mode == context_mode::PLACE_HOLDER;});

    width << val;
}


auto html::elements::html_canvas_element::set_height(
        ulong val)
        -> void
{
    // if this canvas is a placeholder, then the height cannot be set
    dom::helpers::exceptions::throw_v8_exception<INVALID_STATE_ERR>(
            "canvas must not be placeholder in order to change its height",
            [this] {return m_context_mode == context_mode::PLACE_HOLDER;});

    height << val;
}


auto html::elements::html_canvas_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_canvas_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .function("getContext", &html_canvas_element::get_context)
            .function("transferControlToOffscreen", &html_canvas_element::transfer_control_to_offscreen)
            .function("toDataUrl", &html_canvas_element::to_data_url)
            .function("toBlob", &html_canvas_element::to_blob)
            .var("width", &html_canvas_element::width)
            .var("width", &html_canvas_element::height)
            .auto_wrap_objects();
}
