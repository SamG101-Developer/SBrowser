#include "canvas_internals.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/image_bitmap_rendering_context.hpp>

#include <html/elements/html_canvas_element.hpp>


auto html::helpers::canvas_internals::_2d_context_creation_algorithm(
        elements::html_canvas_element* canvas_element,
        const ext::string_any_map& options)
        -> canvas::canvas_rendering_context_2d*
{
    // create the context and assign the canvas element to it
    auto* context = new canvas::canvas_rendering_context_2d{};
    context->canvas = canvas_element;

    // link the bitmap so that it is shared, and set the image bitmap dimensions
    context->m_output_bitmap = canvas_element->m_bitmap;
    set_bitmap_dimensions(canvas_element->width, canvas_element->height);

    // set the context attributes onto the rendering context
    context->m_alpha = options.has_key("alpha") ? options.at("alpha").to<bool>() : true;
    context->m_desynchronized = options.has_key("desynchronized") ? options.at("desynchronized").to<bool>() : false;
    context->m_color_space = options.has_key("colorSpace") ? options.at("colorSpace").to<ext::string>() : "srgb";
    context->m_will_read_frequently = options.has_key("willReadFrequently") ? options.at("willReadFrequently").to<bool>() : false;

    // return the context
    return context;
}


auto html::helpers::canvas_internals::set_bitmap_dimensions(
        elements::html_canvas_element* canvas_element,
        ulong width,
        ulong height)
        -> void
{
    reset_canvas_context_to_default_state(canvas_element);
    canvas_element->m_bitmap->width = width;
    canvas_element->m_bitmap->height = height;

    if (canvas_element->width != width)
        canvas_element->width = width;

    if (canvas_element->height != height)
        canvas_element->height = height;
}


template <typename T>
auto html::helpers::canvas_internals::ensure_there_is_subpath(
        const typename canvas::mixins::canvas_path<T>::point_t& point,
        const canvas::mixins::canvas_path<T>* path)
        -> void
{
    if (path->m_need_new_subpath)
    {
        path->move_to(point.x, point.y);
        path->m_need_new_subpath = false;
    }
}


auto html::helpers::canvas_internals::are_collinear(
        const internal::point& p1,
        const internal::point& p2,
        const internal::point& p3)
        -> bool
{
    auto area =
            p1.x * (p2.y - p3.y) +
            p2.x * (p3.y - p1.y) +
            p3.x * (p1.y - p2.y);

    return area == 0;
}


auto html::helpers::canvas_internals::set_image_bitmap_rendering_contexts_output_bitmap(
        canvas::image_bitmap_rendering_context* context,
        canvas::paint::image_bitmap* bitmap) -> void
{
    context->m_bitmap_mode = false;
    auto canvas = context->canvas;

    // context->m_output_bitmap TODO : set to transparent black
    context->m_output_bitmap.m_origin_clean_flag = true;

    if (bitmap)
    {
        context->m_bitmap_mode = true;
        context->m_output_bitmap.bitmap = bitmap;
    }
}
