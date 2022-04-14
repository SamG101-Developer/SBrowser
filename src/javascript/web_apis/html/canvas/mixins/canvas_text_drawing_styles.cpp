#include "canvas_text_drawing_styles.hpp"

#include <html/canvas/canvas_rendering_context_2d.hpp>


template <typename T>
html::canvas::mixins::canvas_text_drawing_styles<T>::canvas_text_drawing_styles()
{
    // constrain the property values
    direction.constrain_values({"ltr", "rtl", "inherit"});
    text_align.constrain_values({"start", "end", "left", "right", "center"});
    text_baseline.constrain_values({"top", "hanging", "middle", "alphabetical", "ideographic", "bottom"});
    text_rendering.constrain_values({"auto", "optimizeSpeed", "optimizeLegibility", "geometricPrecision"});
    font_kerning.constrain_values({"auto", "normal", "none"});
    font_stretch.constrain_values({"ultra-condensed", "extra-condensed", "condensed", "semi-condensed", "normal", "semi-expanded", "expanded", "extra-expanded", "ultra-expanded"});
    font_variant_caps.constrain_values({"normal", "small-caps", "all-small-caps", "petite-caps", "all-petite-caps", "unicase", "titling-caps"});
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
