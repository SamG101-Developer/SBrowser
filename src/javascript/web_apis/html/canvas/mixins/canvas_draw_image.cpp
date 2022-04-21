#include "canvas_draw_image.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/offscreen_canvas_rendering_context_2d.hpp>


template <typename T>
auto html::canvas::mixins::canvas_draw_image<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    #define func_t_1 void(canvas_draw_image<T>::*)(paint::canvas_image_source*, double, double, double, double)
    #define func_t_2 void(canvas_draw_image<T>::*)(paint::canvas_image_source*, double, double, double, double, double, double, double, double)

    return v8pp::class_<canvas_draw_image<T>>{isolate}
            .template function<func_t_1>("drawImage", &canvas_draw_image<T>::draw_image)
            .template function<func_t_2>("drawImage", &canvas_draw_image<T>::draw_image)
            .auto_wrap_objects();

    #undef func_t_1
    #undef func_t_2
}


template class html::canvas::mixins::canvas_draw_image<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_draw_image<html::canvas::offscreen_canvas_rendering_context_2d>;
