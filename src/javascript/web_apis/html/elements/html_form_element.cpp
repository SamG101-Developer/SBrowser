#include "html_form_element.hpp"

#include <dom/helpers/exceptions.hpp>

#include <html/elements/html_button_element.hpp>
#include <html/helpers/custom_html_elements.hpp>
#include <html/helpers/form_internals.hpp>

// TODO : tidy


html::elements::html_form_element::html_form_element()
        : html_element()
        , mixins::targetable<html_form_element>()
        , mixins::validatable<html_form_element>()
        , ext::listlike<dom::nodes::element*>(elements)
{
    // constrain the property values
    autocomplete.constrain_values({"on", "off"});
    rel.constrain_values({"noreferrer", "nooopener", "opener"});
    enctype.constrain_values({"application/x-www-form-urlencoded", "multipart/form-data", "text/plain"});


    // set the property's values
    accept_charset << "UTF-8"; // TODO : make operator=(...) case insensitive for it?
    elements << new ext::vector<html_form_element*>{};
}


auto html::elements::html_form_element::submit()
        -> void
{
    // submit the form the helper method
    helpers::form_internals::submit(this, this, true);
}


auto html::elements::html_form_element::requestSubmit(
        html_element* submitter)
        -> void
{
    // verify that the submitter is a valid element (button)
    if (submitter)
    {
        // if the submitter element isn't a button, then throw a javascript type error
        dom::helpers::exceptions::throw_v8_exception<V8_TYPE_ERROR>(
                "submitter element must be a HTMLButtonElement",
                [submitter] {return not dynamic_cast<html_button_element*>(submitter);});

        // if the submitter's form owner isn't this form, then throw a not found error
        dom::helpers::exceptions::throw_v8_exception<NOT_FOUND_ERR>(
                "submitter's form owner must be this form",
                [submitter, this] {return dynamic_cast<mixins::form_associated<decltype(submitter)>>(submitter)->form != this;});
    }
    else
        // set the submitter to this if it is empty
        submitter = this;

    // submit the form using the helper method
    helpers::form_internals::submit_form(this, submitter);
}


auto html::elements::html_form_element::reset()
        -> void
{
    // return if the form is locked for reset
    if (m_locked_for_reset) return;

    // set the m_locked_for_reset flag, reset the form, then unset the m_locked_for_reset flag
    m_locked_for_reset = true;
    helpers::form_internals::reset_form(this);
    m_locked_for_reset = false;
}


auto html::elements::html_form_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_form_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .inherit<mixins::targetable<html_form_element>>()
            .inherit<mixins::validatable<html_form_element>>()
            .inherit<ext::listlike<dom::nodes::element*>>()
            .function("auto submit", &html_form_element::submit)
            .function("auto reset", &html_form_element::reset)
            .function("auto requestSubmit", &html_form_element::requestSubmit)
            .var("acceptCharset", &html_form_element::accept_charset)
            .var("action", &html_form_element::action)
            .var("autocomplete", &html_form_element::autocomplete)
            .var("enctype", &html_form_element::enctype)
            .var("encoding", &html_form_element::encoding)
            .var("method", &html_form_element::method)
            .var("name", &html_form_element::name)
            .var("noValidate", &html_form_element::noValidate)
            .var("elements", &html_form_element::elements)
            .auto_wrap_objects();
}
