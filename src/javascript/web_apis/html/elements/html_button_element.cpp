#include "html_button_element.hpp"

#include <ext/string_switch.hpp>

#include <html/helpers/document_internals.hpp>
#include <html/helpers/form_internals.hpp>

#include <QtCore/QPointer>
#include <QtWidgets/QVBoxLayout>


html::elements::html_button_element::html_button_element()
        : html_element{}
        , mixins::validatable<html_button_element>{}
        , mixins::form_associated<html_button_element>{}
{
    // constrain the property values
    type.constrain_values({"submit", "reset", "button"});

    // set the custom accessors
    type.setter = [this](auto && PH1) {set_type(std::forward<decltype(PH1)>(PH1));};

    // create the widget representation
    auto widget = QPointer<QPushButton>{};
    widget->setLayout(QPointer<QVBoxLayout>{});
    m_rendered_widget = widget;

    // attach the qt functions
    value.attach_qt_updater(&QPushButton::setText, widget);
    disabled.attach_qt_updater(&QPushButton::setDisabled, widget);
}


auto html::elements::html_button_element::activation_behaviour(
        dom::events::event* event)
        -> void
{
    // return if the element is disabled or doesn't have a form owner
    if (disabled or not m_form_owner)
        return;

    // return if the owner_document isn't the active document
    if (helpers::document_internals::is_active_document(owner_document))
        return;

    // switch on the type of button
    string_switch(type->c_str())
    {
        // a submit button submits the form from the button
        string_case("submit"):
            helpers::form_internals::submit(m_form_owner, this);
            return;

        // a reset button resets the form
        string_case("reset"):
            helpers::form_internals::reset(m_form_owner);
            return;

        // the normal button doesn't do anything
        default:
            return;
    }
}


auto html::elements::html_button_element::set_type(
        const ext::string& val)
        -> void
{
    // set the form attributes to empty if the button is a submit button
    if (val == "submit")
    {
        form_action = "";
        form_enctype = "";
        form_method = "";
        form_no_validate = "";
        form_target = "";
    }
}


auto html::elements::html_button_element::qt() const
-> QPushButton* {

    // cast to QPushButton
    return qobject_cast<QPushButton*>(m_rendered_widget);
}


auto html::elements::html_button_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_button_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .inherit<mixins::validatable<html_button_element>>()
            .inherit<mixins::form_associated<html_button_element>>()
            .var("form_action", &html_button_element::form_action)
            .var("form_enctype", &html_button_element::form_enctype)
            .var("form_method", &html_button_element::form_method)
            .var("form_target", &html_button_element::form_target)
            .var("name", &html_button_element::name)
            .var("type", &html_button_element::type)
            .var("value", &html_button_element::value)
            .var("form_no_validate", &html_button_element::form_no_validate)
            .var("disabled", &html_button_element::disabled)
            .var("labels", &html_button_element::labels)
            .auto_wrap_objects();
}
