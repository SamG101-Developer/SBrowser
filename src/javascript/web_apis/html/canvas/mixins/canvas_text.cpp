#include "canvas_text.hpp"

#include <ext/infinity.hpp>

#include <html/canvas/canvas_rendering_context_2d.hpp>
#include <html/canvas/offscreen_canvas_rendering_context_2d.hpp>
#include <html/canvas/paint/text_metrics.hpp>
#include <html/helpers/canvas_internals.hpp>

#include <render/painting/painter_path.hpp>


template <typename T>
auto html::canvas::mixins::canvas_text<T>::fill_text(
        const ext::string& text,
        const double x,
        const double y,
        std::optional<double> max_width) -> void
{
    if (ext::infinity<double>::is_inf_or_nan(x, y, max_width.has_value() ? max_width.value() : 0)) return;

    render::painting::painter_path text_path;
    auto glyphs = helpers::canvas_internals::text_preparation_algorithm(text, static_cast<T*>(this), max_width);
    glyphs.template for_each([x, y](internal::glyph& glyph) {glyph.x += x; glyph.y += y;});
    glyphs.template for_each([this, &text_path](internal::glyph& glyph) {text_path.add_text({glyph.x, glyph.y}, static_cast<T*>(this)->m_font, (QString)glyph.text);}); // TODO : transformation
    static_cast<T*>(this)->m_painter.fill_path(text_path, static_cast<T*>(this)->fill_style);
}


template <typename T>
auto html::canvas::mixins::canvas_text<T>::stroke_text(
        const ext::string& text,
        const double x,
        const double y,
        std::optional<double> max_width) -> void
{
    // TODO
}


template <typename T>
auto html::canvas::mixins::canvas_text<T>::measure_text(
        const ext::string& text)
        -> paint::text_metrics
{
    auto glyph = helpers::canvas_internals::text_preparation_algorithm(text, static_cast<T*>(this)).front();

    paint::text_metrics metrics;
    // TODO
}


template <typename T>
auto html::canvas::mixins::canvas_text<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_text<T>>{isolate}
            .template inherit<dom_object>()
            .template function("fillText", &canvas_text<T>::fill_text)
            .template function("strokeText", &canvas_text<T>::stroke_text)
            .template function("measureText", &canvas_text<T>::measure_text)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_text<html::canvas::canvas_rendering_context_2d>;
template class html::canvas::mixins::canvas_text<html::canvas::offscreen_canvas_rendering_context_2d>;
