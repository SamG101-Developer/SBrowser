#include "canvas_user_interface.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template <typename T>
auto html::canvas::mixins::canvas_user_interface<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    #define FUNC_T_1 void(canvas_user_interface<T>::*)(dom::nodes::element*)
    #define FUNC_T_2 void(canvas_user_interface<T>::*)(paint::path_2d*, dom::nodes::element*)
    #define FUNC_T_3 void(canvas_user_interface<T>::*)()
    #define FUNC_T_4 void(canvas_user_interface<T>::*)(paint::path_2d*)

    return v8pp::class_<canvas_user_interface<T>>{isolate}
            .template function<FUNC_T_1>("drawFocusIfNeeded", &canvas_user_interface<T>::draw_focus_if_needed)
            .template function<FUNC_T_2>("drawFocusIfNeeded", &canvas_user_interface<T>::draw_focus_if_needed)
            .template function<FUNC_T_3>("scrollPathIntoView", &canvas_user_interface<T>::scroll_path_into_view)
            .template function<FUNC_T_4>("scrollPathIntoView", &canvas_user_interface<T>::scroll_path_into_view)
            .auto_wrap_objects();

    #undef FUNC_T_1
    #undef FUNC_T_2
    #undef FUNC_T_3
    #undef FUNC_T_4
}


template class html::canvas::mixins::canvas_user_interface<html::canvas::canvas_rendering_context_2d>;
