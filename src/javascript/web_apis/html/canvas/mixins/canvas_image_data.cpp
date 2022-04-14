#include "canvas_image_data.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


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
