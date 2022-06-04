#include "canvas_filters.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/offscreen_canvas_rendering_context_2d.hpp>


template <typename T>
html::canvas::mixins::canvas_filters<T>::canvas_filters()
{
    // set the property's values
    filter << "none";
}


template <typename T>
auto html::canvas::mixins::canvas_filters<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_filters<T>>{isolate}
            .template inherit<dom_object>()
            .template var("filter", &canvas_filters<T>::filter)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_filters<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_filters<html::canvas::offscreen_canvas_rendering_context_2d>;
