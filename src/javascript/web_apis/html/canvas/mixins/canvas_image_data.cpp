#include "canvas_image_data.hpp"

#include <dom/helpers/exceptions.hpp>

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/offscreen_canvas_rendering_context_2d.hpp>
#include <html/canvas/paint/image_data.hpp>


template <typename T>
auto html::canvas::mixins::canvas_image_data<T>::get_image_data(
        const long sx,
        const long sy,
        const long sw,
        const long sh,
        const ext::string_any_map& settings)
        -> paint::image_data
{
    dom::helpers::exceptions::throw_v8_exception<SECURITY_ERR>(
            "rendering context's origin clean flag must be set",
            [this] {return not static_cast<T*>(this)->m_output_bitmap.m_origin_clean_flag;});

    paint::image_data new_image{static_cast<ulong>(sw), static_cast<ulong>(sh), settings};
    new_image.color_space = static_cast<T*>(this)->m_color_space;

    // TODO

}


template <typename T>
auto html::canvas::mixins::canvas_image_data<T>::create_image_data(
        const long sw,
        const long sh,
        const ext::string_any_map& settings)
        -> paint::image_data
{
    paint::image_data new_image{static_cast<ulong>(sw), static_cast<ulong>(sh), settings};
    new_image.color_space = static_cast<T*>(this)->m_color_space;
    return new_image;
}


template <typename T>
auto html::canvas::mixins::canvas_image_data<T>::create_image_data(
        paint::image_data* image_data)
        -> paint::image_data
{
    paint::image_data new_image{image_data->width, image_data->height};
    new_image.color_space = static_cast<T*>(this)->m_color_space;
    return new_image;
}


template <typename T>
auto html::canvas::mixins::canvas_image_data<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    #define FUNC_T_1 paint::image_data(canvas_image_data<T>::*)(long, long, const ext::string_any_map&)
    #define FUNC_T_2 paint::image_data(canvas_image_data<T>::*)(paint::image_data*)
    #define FUNC_T_3 void(canvas_image_data<T>::*)(paint::image_data*, long, long)
    #define FUNC_T_4 void(canvas_image_data<T>::*)(paint::image_data*, long, long, long, long, long, long)

    return v8pp::class_<canvas_image_data<T>>{isolate}
            .template inherit<dom_object>()
            .template function("getImageData", &canvas_image_data<T>::get_image_data)
            .template function<FUNC_T_1>("createImageData", &canvas_image_data<T>::create_image_data)
            .template function<FUNC_T_2>("createImageData", &canvas_image_data<T>::create_image_data)
            .template function<FUNC_T_3>("putImageData", &canvas_image_data<T>::put_image_data)
            .template function<FUNC_T_4>("putImageData", &canvas_image_data<T>::put_image_data)
            .auto_wrap_objects();

    #undef FUNC_T_1
    #undef FUNC_T_2
    #undef FUNC_T_3
    #undef FUNC_T_4
}


template class html::canvas::mixins::canvas_image_data<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_image_data<html::canvas::offscreen_canvas_rendering_context_2d>;
