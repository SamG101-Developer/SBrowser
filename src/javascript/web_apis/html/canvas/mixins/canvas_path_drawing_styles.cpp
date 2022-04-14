#include "canvas_path_drawing_styles.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template <typename T>
html::canvas::mixins::canvas_path_drawing_styles<T>::canvas_path_drawing_styles()
{
    // constrain the property values
    line_cap.constrain_values({"butt", "round", "square"});
    line_join.constrain_values({"round", "bevel", "miter"});

    // set the property values
    line_width << 1.0;
    miter_limit << 10.0;
    line_cap << "butt";
    line_join << "miter";

}


template <typename T>
auto html::canvas::mixins::canvas_path_drawing_styles<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_path_drawing_styles<T>>{isolate}
            .template inherit<dom_object>()
            .template function("get_line_dash", &canvas_path_drawing_styles<T>::get_line_dash)
            .template function("set_line_dash", &canvas_path_drawing_styles<T>::set_line_dash)
            .template var("line_dash_offset", &canvas_path_drawing_styles<T>::line_dash_offset)
            .template var("line_width", &canvas_path_drawing_styles<T>::line_width)
            .template var("miter_limit", &canvas_path_drawing_styles<T>::miter_limit)
            .template var("line_cap", &canvas_path_drawing_styles<T>::line_cap)
            .template var("line_join", &canvas_path_drawing_styles<T>::line_join)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_path_drawing_styles<html::canvas::canvas_rendering_context_2d>;
