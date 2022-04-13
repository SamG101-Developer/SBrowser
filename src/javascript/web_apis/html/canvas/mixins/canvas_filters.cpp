#include "canvas_filters.hpp"


template <typename T>
html::canvas::mixins::canvas_filters<T>::canvas_filters()
{
    // set the properties
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
