#ifndef SBROWSER_CSS_SHORTHAND_PROPERTY_HPP
#define SBROWSER_CSS_SHORTHAND_PROPERTY_HPP

#include <ext/css_property.hpp>

namespace ext {struct css_shorthand_property;}
namespace {auto contained_items(ext::css_shorthand_property& property, const ext::string& shorthand) -> ext::vector<ext::css_property*>;}


struct ext::css_shorthand_property final : public property<ext::string>
{
public constructors:
    explicit css_shorthand_property(string&& name);

public js_methods:
    auto add_css_property(css_property& css_longhand_property) -> css_shorthand_property&;

public operators:
    auto operator=(const string& o) -> css_shorthand_property& override;
    operator string() const override;

public cpp_properties:
    vector<css_property> css_properties;

private cpp_properties:
    string m_name;
    string m_initial;
};


ext::css_shorthand_property::css_shorthand_property(string&& name)
        : property<ext::string>()
        , m_name(name)
{}


auto ext::css_shorthand_property::add_css_property(css_property& css_longhand_property) -> ext::css_shorthand_property&
{
    // append the property and update the initial string to include it ie for margins [0px 0px 0px -> 0px 0px 0px 0px]
    css_properties.append(css_longhand_property);
    m_initial += (string)" " + css_longhand_property.m_initial;

    // return a reference to the css shorthand property
    return *this;
}


auto ext::css_shorthand_property::operator=(const string& o) -> ext::css_shorthand_property&
{
    // extract the contained values from the string, and set them sequentially to each property in the list
    auto sub_items = ::contained_items(*this, o);
    css_properties.for_each([&sub_items](css_property* longhand_property) {
        *longhand_property << *sub_items.front();
        sub_items.pop(0);
    });

    // return the reference to the property
    return *this;
}


ext::css_shorthand_property::operator string() const
{
    // create a joined string and append the string representations of the longhand properties to it sequentially
    ext::string joined;
    return joined = css_properties
            .transform<string>([](css_property* css_longhand_property) -> string {return *css_longhand_property;})
            .join();
}


auto contained_items(ext::css_shorthand_property& property, const ext::string& shorthand) -> ext::vector<ext::css_property*>
{
    // split the items out of the string, and determine how many there should be for the given property
    auto sub_items = shorthand.split(' ');
    auto sub_items_desired_length = property.css_properties.length();

    // if there are fewer items than desired then return the first item duplicated n times, otherwise the split items
    return sub_items.length() < sub_items_desired_length
            ? ext::string_vector{sub_items.front()} * sub_items_desired_length
            : sub_items;
}



#endif //SBROWSER_CSS_SHORTHAND_PROPERTY_HPP
