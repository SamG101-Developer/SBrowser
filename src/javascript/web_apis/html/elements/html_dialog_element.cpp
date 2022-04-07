#include "html_dialog_element.hpp"

#include <dom/helpers/exceptions.hpp>
#include <dom/helpers/mutation_observers.hpp>
#include <dom/helpers/shadows.hpp>

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_dialog_element::html_dialog_element()
        : html_element{}
{
    // set the properties
    return_value << "";

    // create the widget representation
    m_rendered_widget = QPointer<QDialog>{};


}


auto html::elements::html_dialog_element::show()
        -> void
{
    // return if the dialog is already open
    if (open) return;

    // reset the open property and focus on the previously_focused_element (focus ordering)
    open = false;
    m_previously_focused_element->qt()->setFocus();

    // focus on the dialog
    html::helpers::focusing::dialog_focusing_steps(this);
}


auto html::elements::html_dialog_element::show_modal()
        -> void
{
    // if this dialog is already open, throw an invalid state error
    dom::helpers::exceptions::throw_v8_exception<INVALID_STATE_ERR>(
            "subject must not have an open attribute set in order to show dialog in modal mode",
            [this] {return open;});

    // if this dialog is not connected, throw an invalid state error
    dom::helpers::exceptions::throw_v8_exception<INVALID_STATE_ERR>(
            "subject must be connected in order to show dialog in modal mode",
            [this] {return not dom::helpers::shadows::is_connected(this);});

    // reset the open property and set the m_modal_flag to true
    open = false;
    m_modal_flag = true;

    helpers::dialogs::block_document_by_modal_dialog(owner_document, this);
    // TODO -> top layer stuff (fullscreen API) ie show the dialog

    // focus on the previously_focused_element (focus ordering)
    m_previously_focused_element->qt()->setFocus();

    // focus on the dialog
    html::helpers::focusing::dialog_focusing_steps(this);
}


auto html::elements::html_dialog_element::close(
        const ext::string& result)
        -> void {

    // if this dialog is already closed, then return early
    if (not open) return;
    open = false;

    return_value = result ? result : return_value;
    m_modal_flag = false;

    // TODO -> top layer stuff (fullscreen API) ie close the dialog

    if (m_previously_focused_element)
    {
        auto element = dom::nodes::element{*m_previously_focused_element};
        m_previously_focused_element = nullptr;
        html::helpers::focusing::focus_steps(element);
    }

    dom::helpers::mutation_observers::queue_element_task(); // TODO : arguments
}


auto html::elements::html_dialog_element::qt() const
        -> QDialog*
{
    return qobject_cast<QDialog*>(m_rendered_widget);
}


auto html::elements::html_dialog_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_dialog_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .function("show", &html_dialog_element::show)
            .function("showModal", &html_dialog_element::show_modal)
            .function("close", &html_dialog_element::close)
            .var("open", &html_dialog_element::open)
            .var("returnValue", &html_dialog_element::return_value)
            .auto_wrap_objects();
}
