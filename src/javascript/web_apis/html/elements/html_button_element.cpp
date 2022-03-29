#include "html_button_element.hpp"

#include <ext/string_switch.hpp>

#include <html/helpers/custom_html_elements.hpp>

#include <QtCore/QPointer>
#include <QtWidgets/QVBoxLayout>


html::elements::html_button_element::html_button_element()
        : html_element{}
        , mixins::validatable{}
{
    // set the properties
    labels = new ext::vector<dom::nodes::node*>{};

    // create the widget representation
    auto widget = QPointer<QPushButton>{};
    widget->setLayout(QPointer<QVBoxLayout>{});
    m_rendered_widget = widget;

    // attach the qt functions
    value.attach_qt_updater(&QPushButton::setText, widget);
    disabled.attach_qt_updater(&QPushButton::setDisabled, widget);

    HTML_CONSTRUCTOR
}


auto html::elements::html_button_element::render() const
        -> QPushButton* {

    // cast to QPushButton
    return qobject_cast<QPushButton*>(m_rendered_widget);
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
