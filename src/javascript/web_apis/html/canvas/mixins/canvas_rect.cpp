#include "canvas_rect.hpp"

#include <ext/infinity.hpp>

#include <html/canvas/canvas_rendering_context_2d.hpp>


template <typename T>
auto html::canvas::mixins::canvas_rect<T>::clear_rect(
        const double x,
        const double y,
        const double w,
        const double h)
        -> void
{
    if (ext::infinity<double>::is_inf_or_nan(x, y, w, h)) return;

    static_cast<T*>(this)->m_painter.eraseRect(x, y, w, h);
}


template <typename T>
auto html::canvas::mixins::canvas_rect<T>::fill_rect(
        const double x,
        const double y,
        const double w,
        const double h)
        -> void
{
    if (ext::infinity<double>::is_inf_or_nan(x, y, w, h)) return;

    auto fill_style = static_cast<T*>(this)->fill_style;
    static_cast<T*>(this)->m_painter.fill_rect(x, y, w, h, fill_style);
}


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


template class html::canvas::mixins::canvas_rect<html::canvas::canvas_rendering_context_2d>;
