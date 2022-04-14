#pragma once
#ifndef SBROWSER_CANVAS_TEXT_DRAWING_STYLES_HPP
#define SBROWSER_CANVAS_TEXT_DRAWING_STYLES_HPP

#include <dom_object.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_text_drawing_styles;}


template <typename T>
class html::canvas::mixins::canvas_text_drawing_styles : public virtual dom_object
{
public constructors:
    canvas_text_drawing_styles();

public js_properties:
    ext::html_property<ext::string, _F> direction;
    ext::html_property<ext::string, _F> letter_spacing;
    ext::html_property<ext::string, _F> word_spacing;
    ext::html_property<ext::string, _F> text_align;
    ext::html_property<ext::string, _F> text_baseline;
    ext::html_property<ext::string, _F> text_rendering;
    ext::html_property<ext::string, _F> font;
    ext::html_property<ext::string, _F> font_kerning;
    ext::html_property<ext::string, _F> font_stretch;
    ext::html_property<ext::string, _F> font_variant_caps;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_TEXT_DRAWING_STYLES_HPP
