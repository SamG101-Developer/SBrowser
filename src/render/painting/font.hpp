#pragma once
#ifndef SBROWSER_FONT_HPP
#define SBROWSER_FONT_HPP

#include <ext/decorators.hpp>
#include <ext/map.hpp>
#include <ext/string.hpp>

#include <QtGui/QFont>
#include <QtGui/QFontMetricsF>

namespace render::painting {class font;}


class render::painting::font
{
public constructors:
    font();

public cpp_methods:
    auto set_letter_spacing(const ext::string& letter_spacing) -> void;
    auto set_word_spacing(const ext::string& word_spacing) -> void;
    auto set_text_align(const ext::string& text_align) -> void;
    auto set_text_rendering(const ext::string& text_rendering) -> void;
    auto set_baseline(const ext::string& baseline) -> void;
    auto set_font(const ext::string& font) -> void;
    auto set_kerning(const ext::string& kerning) -> void;
    auto set_stretch(const ext::string& stretch) -> void;
    auto set_variant_caps(const ext::string& variant_caps) -> void;

public cpp_methods:
    auto baseline() -> double;

public cpp_properties:
    ext::map<ext::string, QFont::StyleStrategy> translate_text_rendering;
    ext::map<ext::string, bool> translate_kerning;
    ext::map<ext::string, QFont::Stretch> translate_stretch;
    ext::map<ext::string, QFont::Capitalization> translate_variant_caps;

public operators:
    operator QFont() const;

private cpp_properties:
    QFont m_font;
    QFontMetricsF m_metrics;

    double m_baseline;
};


#endif //SBROWSER_FONT_HPP
