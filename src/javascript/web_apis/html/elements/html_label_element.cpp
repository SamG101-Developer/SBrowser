#include "html_label_element.hpp"

#include <html/mixins/form_associated.hpp>


html::elements::html_label_element::html_label_element()
        : html_element{}
        , mixins::form_associated<html_label_element>{}
{
    // set the custom accessor methods
    form.getter     = [this] {return get_form();};
    html_for.setter = [this](auto && PH1) {set_html_for(std::forward<decltype(PH1)>(PH1));};
}


auto html::elements::html_label_element::get_form() const
        -> html_form_element*
{
    // TODO
    if (html_for->empty())
        return nullptr;

    if (not form_associated_elements.contains(html_for))
        return nullptr;
}


auto html::elements::html_label_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_label_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .inherit<mixins::form_associated<html_label_element>>()
            .var("htmlFor", &html_label_element::html_for)
            .var("control", &html_label_element::control)
            .var("form", &html_label_element::form)
            .auto_wrap_objects();
}
