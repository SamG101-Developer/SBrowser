#ifndef SBROWSER_CSS_SHORTHAND_PROPERTY2_HPP
#define SBROWSER_CSS_SHORTHAND_PROPERTY2_HPP

#include <ext/properties/css_property2.hpp>

namespace ext {struct css_shorthand_property2;}
namespace {ext::vector<ext::css_property2*> contained_items(ext::css_shorthand_property2& property, ext::cstring& shorthand);}


struct ext::css_shorthand_property2 : property2<ext::string> {
public: constructors
    css_shorthand_property2(string&& name);

public: methods
    css_shorthand_property2& add_css_property(css_property2& css_longhand_property);
    operator string() const override;

public: operators
    css_shorthand_property2& operator=(cstring& o) override;

public: internal_properties
    vector<css_property2> css_properties;

private internal_properties:
    string m_name;
    string m_initial;
};


ext::css_shorthand_property2::css_shorthand_property2(string&& name)
        : property2<ext::string>()
        , m_name(name)
{}


ext::css_shorthand_property2& ext::css_shorthand_property2::add_css_property(css_property2& css_longhand_property)
{
    // append the property and update the initial string to include it ie for margins [0px 0px 0px -> 0px 0px 0px 0px]
    css_properties.append(css_longhand_property);
    m_initial += (string)" " + css_longhand_property.m_initial;

    // return a reference to the css shorthand property
    return *this;
}


ext::css_shorthand_property2::operator string() const
{
    // create a joined string and append the string representations of the longhand properties to it sequentially
    ext::string joined;
    return joined = css_properties
            .transform<string>([](css_property2* css_longhand_property) -> string {return *css_longhand_property;})
            .join();
}


ext::css_shorthand_property2& ext::css_shorthand_property2::operator=(cstring& o)
{
    // extract the contained values from the string, and set them sequentially to each property in the list
    auto sub_items = ::contained_items(*this, o);
    css_properties.for_each([&sub_items](css_property2* longhand_property) {
        *longhand_property << *sub_items.front();
        sub_items.pop(0);
    });

    // return the reference to the property
    return *this;
}


ext::vector<ext::css_property2*> contained_items(ext::css_shorthand_property2& property, ext::cstring& shorthand) {
    // split the items out of the string, and determine how many there should be for the given property
    auto sub_items = shorthand.split(' ');
    auto sub_items_desired_length = property.css_properties.length();

    // if there are fewer items than desired then return the first item duplicated n times, otherwise the split items
    return sub_items.length() < sub_items_desired_length
            ? ext::vector<ext::string>{sub_items.front()} * sub_items_desired_length
            : sub_items;
}



#endif //SBROWSER_CSS_SHORTHAND_PROPERTY2_HPP
