#include "html_datalist_element.hpp"

#include <html/helpers/custom_html_elements.hpp>

#include <QtWidgets/QMenu>


html::elements::html_datalist_element::html_datalist_element()
        : html_element()
{
    // set the properties
    local_name = "datalist";

    auto widget = QPointer<QPushButton>{};
    widget->setMenu(new QMenu{});
    m_rendered_widget = widget;

    HTML_CONSTRUCTOR
}


auto html::elements::html_datalist_element::render() const
        -> QPushButton*
{
    return qobject_cast<QPushButton*>(m_rendered_widget);
}
