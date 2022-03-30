#include "html_form_element.hpp"

#include <dom/helpers/exceptions.hpp>
#include <html/elements/html_button_element.hpp>
#include <html/helpers/custom_html_elements.hpp>


html::elements::html_form_element::html_form_element()
        : html_element{}
{
    local_name = "";

    HTML_CONSTRUCTOR
}


auto html::elements::html_form_element::submit()
        -> void
{
    // submit the form the helper method
    helpers::form_controls::submit_form(this, this, true);
}


auto html::elements::html_form_element::requestSubmit(
        html_element* submitter)
        -> void
{
    // verify that the submitter is a valid element (button)
    if (submitter)
    {
        // if the submitter element isn't a button, then throw a javascript type error
        dom::helpers::exceptions::throw_v8_exception(
                "submitter element must be a HTMLButtonElement",
                v8::Exception::TypeError,
                [submitter] {return not dynamic_cast<html_button_element*>(submitter);});

        // if the submitter's form owner isn't this form, then throw a not found error
        dom::helpers::exceptions::throw_v8_exception(
                "submitter's form owner must be this form",
                NOT_FOUND_ERR,
                [submitter, this] {return submitter->m_form_owner != this;});
    }
    else
        // set the submitter to this if it is empty
        submitter = this;

    // submit the form using the helper method
    helpers::form_controls::submit_form(this, submitter);
}


auto html::elements::html_form_element::reset()
        -> void
{
    // return if the form is locked for reset
    if (m_locked_for_reset) return;

    // set the m_locked_for_reset flag, reset the form, then unset the m_locked_for_reset flag
    m_locked_for_reset = true;
    helpers::form_controls::reset_form(this);
    m_locked_for_reset = false;
}
