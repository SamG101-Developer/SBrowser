#include "html_style_element.hpp"


html::elements::html_style_element::html_style_element()
        : html_element{}
{
    // set the custom accessor methods
    disabled.getter = [this] {return get_disabled();};
    disabled.setter = [this](auto && PH1) {set_disabled(std::forward<decltype(PH1)>(PH1));};

    // set the property's values
    media = "all";
}


auto html::elements::html_style_element::get_disabled() const
        -> bool
{
    // TODO : associated stylesheet check
    // TODO : associated stylesheet check
    return false;
}


auto html::elements::html_style_element::set_disabled(
        bool val)
        -> void
{
    // TODO : associated stylesheet check
    // TODO : associated stylesheet check
}


auto html::elements::html_style_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_style_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("disabled", &html_style_element::disabled)
            .var("media", &html_style_element::media)
            .var("blocking", &html_style_element::blocking)
            .auto_wrap_objects();
}
