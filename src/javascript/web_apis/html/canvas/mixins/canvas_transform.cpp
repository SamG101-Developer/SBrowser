#include "canvas_transform.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template <typename T>
auto html::canvas::mixins::canvas_transform<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_transform<T>>{isolate}
            .template inherit<dom_object>()
            .template function("scale", &canvas_transform<T>::scale)
            .template function("rotate", &canvas_transform<T>::rotate)
            .template function("translate", &canvas_transform<T>::translate)
            .template function("transform", &canvas_transform<T>::transform)
            .template function("resetTransformation", &canvas_transform<T>::reset_transformation)
            .template function("setTransformation", &canvas_transform<T>::set_transform)
            .template function("getTransformation", &canvas_transform<T>::get_transform)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_transform<html::canvas::canvas_rendering_context_2d>;
