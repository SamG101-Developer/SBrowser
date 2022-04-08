#include "validatable.hpp"

#include <html/elements/html_button_element.hpp>
#include <html/elements/html_form_element.hpp>
#include <html/elements/html_output_element.hpp>
#include <html/elements/html_select_element.hpp>
#include <html/elements/html_text_area_element.hpp>
#include <html/other/element_internals.hpp>


template<typename T>
html::mixins::validatable<T>::validatable()
        : dom_object()
{
    // constrain the property values
    validity.constrain_values({
        "valueMissing",
        "typeMismatch",
        "patternMismatch",
        "tooLong",
        "tooShort",
        "rangeUnderflow",
        "rangeOverflow",
        "stepMismatch",
        "badInput",
        "customError",
        "valid"
    });
}


template<typename T>
auto html::mixins::validatable<T>::check_validity()
        -> bool
{
    return helpers::validation_internals::statically_validate(static_cast<T*>(this));
}


template<typename T>
auto html::mixins::validatable<T>::report_validity()
        -> bool
{
    return helpers::validation_internals::interactively_validate(static_cast<T*>(this));
}


template<typename T>
auto html::mixins::validatable<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<validatable<T>>{isolate}
            .template inherit<dom_object>()
            .template function("checkValidity", &validatable<T>::check_validity)
            .template function("reportValidity", &validatable<T>::report_validity)
            .template function("setCustomValidity", &validatable<T>::set_custom_validity)
            .template var("willValidate", &validatable<T>::will_validate)
            .template var("validationMessage", &validatable<T>::validation_message)
            .template var("validity", &validatable<T>::validity)
            .auto_wrap_objects();
}


template class html::mixins::validatable<html::elements::html_button_element>;
template class html::mixins::validatable<html::elements::html_form_element>;
template class html::mixins::validatable<html::elements::html_output_element>;
template class html::mixins::validatable<html::elements::html_select_element>;
template class html::mixins::validatable<html::elements::html_text_area_element>;
template class html::mixins::validatable<html::other::element_internals>;
