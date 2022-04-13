#include "canvas_rect.hpp"


template <typename T>
auto html::canvas::mixins::canvas_rect<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_rect<T>>{isolate}
            .template inherit<dom_object>()
            .template function("clearRect", &canvas_rect<T>::clear_rect)
            .template function("fillRect", &canvas_rect<T>::clear_rect)
            .template function("strokeRect", &canvas_rect<T>::clear_rect)
            .auto_wrap_objects();
}
