#include "html_opt_group_element.hpp"


html::elements::html_opt_group_element::html_opt_group_element()
        : html_element()
{
    // set the custom accessor methods
    disabled.setter = [this](auto && PH1) {set_disabled(std::forward<decltype(PH1)>(PH1));};
}


auto html::elements::html_opt_group_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_opt_group_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("disabled", &html_opt_group_element::disabled)
            .var("label", &html_opt_group_element::label)
            .auto_wrap_objects();
}
