#include "html_field_set_element.hpp"

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_field_set_element::html_field_set_element()
        : html_element{}
{
    // set the properties
    type = "fieldset";

    // attach the qt functions
    disabled.attach_qt_updater(&QWidget::setDisabled, this);


}


auto html::elements::html_field_set_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_field_set_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("disabled", &html_field_set_element::disabled)
            .var("form", &html_field_set_element::form)
            .var("name", &html_field_set_element::name)
            .var("type", &html_field_set_element::type)
            .var("elements", &html_field_set_element::elements)
            .auto_wrap_objects();
}
