#include "font.hpp"


render::painting::font::font()
{
    translate_text_rendering =
    {
            {"auto", QFont::StyleStrategy::PreferDefault}, // TODO
            {"optimizeSpeed", QFont::StyleStrategy::NoAntialias}, // TODO
            {"optimizeLegibility", QFont::StyleStrategy::PreferQuality},
            {"geometricPrecision", QFont::StyleStrategy::PreferMatch}
    };

    translate_kerning =
    {
            {"auto", QFont{}.kerning()},
            {"normal", true},
            {"none", false}
    };

    translate_stretch =
    {
            {"ultra-condensed", QFont::Stretch::UltraCondensed},
            {"extra-condensed", QFont::Stretch::ExtraCondensed},
            {"condensed", QFont::Stretch::Condensed},
            {"semi-condensed", QFont::Stretch::SemiCondensed},
            {"normal", QFont::Stretch::Unstretched},
            {"semi-expanded", QFont::Stretch::SemiExpanded},
            {"expanded", QFont::Stretch::Expanded},
            {"extra-expanded", QFont::Stretch::ExtraExpanded},
            {"ultra-expanded", QFont::Stretch::UltraExpanded}
    };

    translate_variant_caps = // TODO -> not all correct
    {
            {"normal", QFont::Capitalization::Capitalize},
            {"small-caps", QFont::Capitalization::SmallCaps},
            {"all-small-caps", QFont::Capitalization::AllLowercase},
            {"petite-caps", QFont::Capitalization::SmallCaps},
            {"all-petite-caps", QFont::Capitalization::AllLowercase},
            {"unicase", QFont::Capitalization::MixedCase},
            {"titling-caps", QFont::Capitalization::Capitalize}
    };
}


auto render::painting::font::set_letter_spacing(
        const ext::string& letter_spacing)
        -> void
{
    // set the letter spacing by removing the 'px' from the string, and use absolute spacing
    auto remove_px = letter_spacing.remove_postfix("px");
    auto parsed_spacing = static_cast<qreal>(remove_px);
    m_font.setLetterSpacing(QFont::SpacingType::AbsoluteSpacing, parsed_spacing);
}


auto render::painting::font::set_word_spacing(
        const ext::string& word_spacing)
        -> void
{
    // set the word spacing by removing 'px' from the string
    auto remove_px = word_spacing.remove_postfix("px");
    auto parsed_spacing = static_cast<qreal>(remove_px);
    m_font.setWordSpacing(parsed_spacing);
}


auto render::painting::font::set_text_rendering(
        const ext::string& text_rendering)
        -> void
{
    // set the text rendering preference
    m_font.setStyleStrategy(translate_text_rendering.at(text_rendering));
}


auto render::painting::font::set_font(
        const ext::string& font)
        -> void
{
    // set the font family directly from the attribute
    m_font.setFamily(static_cast<QString>(font));
}


auto render::painting::font::set_kerning(
        const ext::string& kerning)
        -> void
{
    // set the font kerning
    m_font.setKerning(translate_kerning.at(kerning));
}


auto render::painting::font::set_stretch(
        const ext::string& stretch)
        -> void
{
    // set the font stretch
    m_font.setStretch(translate_stretch.at(stretch));
}


auto render::painting::font::set_variant_caps(
        const ext::string& variant_caps)
        -> void
{
    // set the variant caps
    m_font.setCapitalization(translate_variant_caps.at(variant_caps));
}
