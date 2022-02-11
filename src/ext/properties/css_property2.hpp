#ifndef SBROWSER_CSS_PROPERTY2_HPP
#define SBROWSER_CSS_PROPERTY2_HPP

#include <ext/iterables/string.hpp>
#include <ext/properties/property2.hpp>

namespace ext {
    struct css_property2;
    struct css_shorthand_property2;
}


struct ext::css_property2 : public property2<ext::string> {
public: friends
    friend struct css_shorthand_property2;

public: enums
    enum animation_t {NOT_ANIMATABLE, BY_COMPUTED_VALUE, DISCRETE, REPEATABLE_LIST, NUMBER, LIST};
    enum percentage_t {NA, CORRESPONDING_DIMENSION_CONTENT_AREA, LOGICAL_WIDTH};
    enum canonical_order_t {PER_GRAMMAR};
    enum media_t {VISUAL, HIDDEN};

public: constructors
    css_property2(
            string&& name, string&& initial, bool&& inherited = false, animation_t&& animation_type = NOT_ANIMATABLE,
            percentage_t&& percentage_type = NA, canonical_order_t&& canonical_order_type = PER_GRAMMAR,
            media_t&& media_type = HIDDEN);

private: internal_properties
    string m_name;
    string m_initial;
    bool m_inherited;
    animation_t m_animation_type;
    percentage_t m_percentage_type;
    canonical_order_t m_canonical_order_type;
    media_t m_media_type;
};


ext::css_property2::css_property2(
        string&& name, string&& initial, bool&& inherited, animation_t&& animation_type, percentage_t&& percentage_type,
        canonical_order_t&& canonical_order_type, media_t&& media_type)

        : property2<ext::string>()
        , m_name(name)
        , m_initial(initial)
        , m_inherited(inherited)
        , m_animation_type(animation_type)
        , m_percentage_type(percentage_type)
        , m_canonical_order_type(canonical_order_type)
        , m_media_type(media_type)
{}


#endif //SBROWSER_CSS_PROPERTY2_HPP
