#include "html_datalist_element.hpp"

#include <html/helpers/custom_html_elements.hpp>

#include <QtWidgets/QVBoxLayout>


html::elements::html_datalist_element::html_datalist_element()
        : html_element()
{
    // set the properties
    local_name = "datalist";

    auto widget = QPointer<QComboBox>{};
    widget->setLayout(new QVBoxLayout{});
    m_rendered_widget = widget;

    HTML_CONSTRUCTOR
}


auto html::elements::html_datalist_element::render() const
        -> QComboBox*
{
    return qobject_cast<QComboBox*>(m_rendered_widget);
}
