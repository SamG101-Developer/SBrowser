#include "canvas_text_drawing_styles.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template <typename T>
html::canvas::mixins::canvas_text_drawing_styles<T>::canvas_text_drawing_styles()
{
    // constrain the property values
    direction.constrain_values({"ltr", "rtl", "inherit"});
    text_align.constrain_values({"start", "end", "left", "right", "center"});
    text_baseline.constrain_values({"top", "hanging", "middle", "alphabetical", "ideographic", "bottom"});
    text_rendering.constrain_values(m_font.translate_text_rendering.keys());
    font_kerning.constrain_values(m_font.translate_kerning.keys());
    font_stretch.constrain_values(m_font.translate_stretch.keys());
    font_variant_caps.constrain_values(m_font.translate_variant_caps.keys());

    // attach the qt functions
    letter_spacing.template attach_qt_updater(&render::painting::font::set_letter_spacing, m_font);
    word_spacing.template attach_qt_updater(&render::painting::font::set_word_spacing, m_font);
    text_rendering.template attach_qt_updater(&render::painting::font::set_text_rendering, m_font);
    font_kerning.template attach_qt_updater(&render::painting::font::set_kerning, m_font);
    font_stretch.template attach_qt_updater(&render::painting::font::set_stretch, m_font);
    font_variant_caps.template attach_qt_updater(&render::painting::font::set_variant_caps, m_font);
}


template <typename T>
auto html::canvas::mixins::canvas_text_drawing_styles<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<canvas_text_drawing_styles<T>>{isolate}
            .template inherit<dom_object>()
            .template var("direction", &canvas_text_drawing_styles<T>::direction)
            .template var("letterSpacing", &canvas_text_drawing_styles<T>::letter_spacing)
            .template var("wordSpacing", &canvas_text_drawing_styles<T>::word_spacing)
            .template var("textAlign", &canvas_text_drawing_styles<T>::text_align)
            .template var("textBaseline", &canvas_text_drawing_styles<T>::text_baseline)
            .template var("textRendering", &canvas_text_drawing_styles<T>::text_rendering)
            .template var("font", &canvas_text_drawing_styles<T>::font)
            .template var("fontKerning", &canvas_text_drawing_styles<T>::font_kerning)
            .template var("fontStretch", &canvas_text_drawing_styles<T>::font_stretch)
            .template var("fontVariantCaps", &canvas_text_drawing_styles<T>::font_variant_caps)
            .auto_wrap_objects();
}


template class html::canvas::mixins::canvas_text_drawing_styles<html::canvas::canvas_rendering_context_2d>;
