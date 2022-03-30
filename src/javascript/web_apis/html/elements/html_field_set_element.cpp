#include "html_field_set_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_field_set_element::html_field_set_element()
        : html_element{}
{
    local_name = "fieldset";
    type = "fieldset";

    disabled.attach_qt_updater(&QWidget::setDisabled, this);

    HTML_CONSTRUCTOR
}


auto html::elements::html_field_set_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_field_set_element>{isolate}
            .template ctor<>()
            .template inherit<html_element>()
            .template var("disabled", &html_field_set_element::disabled)
            .template var("form", &html_field_set_element::form)
            .template var("name", &html_field_set_element::name)
            .template var("type", &html_field_set_element::type)
            .template var("elements", &html_field_set_element::elements)
            .auto_wrap_objects();
}
