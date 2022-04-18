#include "canvas_fill_stroke_styles.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/paint/canvas_gradient.hpp>
#include <html/canvas/paint/canvas_pattern.hpp>

#include <QtGui/QLinearGradient>
#include <QtGui/QRadialGradient>
#include <QtGui/QConicalGradient>

template<typename T>
html::canvas::mixins::canvas_fill_stroke_styles<T>::canvas_fill_stroke_styles()
{
    // set the property values
    stroke_style << "black";
    fill_style << "black";
}


template <typename T>
auto html::canvas::mixins::canvas_fill_stroke_styles<T>::create_linear_gradient(
        const double x0,
        const double y0,
        const double x1,
        const double y1)
        -> paint::canvas_gradient
{
    // create a canvas gradient and internal linear gradient
    paint::canvas_gradient gradient;
    auto linear_gradient = new QLinearGradient{};

    // set the attributes of the linear gradient
    linear_gradient->setStart(x0, y0);
    linear_gradient->setFinalStop(x1, y1);
    gradient.m_gradient = linear_gradient;

    // return the gradient
    return gradient;
}


template <typename T>
auto html::canvas::mixins::canvas_fill_stroke_styles<T>::create_radial_gradient(
        const double x0,
        const double y0,
        const double r0,
        const double x1,
        const double y1,
        const double r1)
        -> paint::canvas_gradient
{
    // create a canvas gradient and internal radial gradient
    paint::canvas_gradient gradient;
    auto radial_gradient = new QRadialGradient{};

    // set the attributes of the radial gradient
    radial_gradient->setCenter(x0, y0);
    radial_gradient->setCenterRadius(r0);
    radial_gradient->setFocalPoint(x1, y1); // TODO : correct attribute?
    radial_gradient->setFocalRadius(r1);
    gradient.m_gradient = radial_gradient;

    // return the gradient
    return gradient;
}


template <typename T>
auto html::canvas::mixins::canvas_fill_stroke_styles<T>::create_conic_gradient(
        const double start_angle,
        const double x,
        const double y)
        -> paint::canvas_gradient
{
    // create a canvas gradient and internal conical gradient
    paint::canvas_gradient gradient;
    auto conical_gradient = new QConicalGradient{};

    // set the attributes of the conical gradient
    conical_gradient->setCenter(x, y);
    conical_gradient->setAngle(start_angle);
    gradient.m_gradient = conical_gradient;

    // return the gradient
    return gradient;
}


template<typename T>
auto html::canvas::mixins::canvas_fill_stroke_styles<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_fill_stroke_styles<T>>{isolate}
            .template inherit<dom_object>()
            .template function("createLinearGradient", &canvas_fill_stroke_styles<T>::create_linear_gradient)
            .template function("createRadialGradient", &canvas_fill_stroke_styles<T>::create_radial_gradient)
            .template function("createConicGradient", &canvas_fill_stroke_styles<T>::create_conic_gradient)
            .template function("createPattern", &canvas_fill_stroke_styles<T>::create_pattern)
            .template var("strokeStyle", &canvas_fill_stroke_styles<T>::stroke_style)
            .template var("fillStyle", &canvas_fill_stroke_styles<T>::fill_style)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_fill_stroke_styles<html::canvas::canvas_rendering_context_2d>;
