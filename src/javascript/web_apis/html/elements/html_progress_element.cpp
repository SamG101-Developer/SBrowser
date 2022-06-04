#include "html_progress_element.hpp"

#include <ext/infinity.hpp>


html::elements::html_progress_element::html_progress_element()
{
    // constrain the property values
    value.clamp_values(0, max);

    // set the property's values
    max << ext::infinity<double>{};

    // create the widget representation
    auto widget = QPointer<QProgressBar>{};
    m_rendered_widget = widget;

    // attach the qt functions
    max.template attach_qt_updater(&QProgressBar::setMaximum, widget);
    value.template attach_qt_updater(&QProgressBar::setValue, render());
}


auto html::elements::html_progress_element::qt() const
        -> QProgressBar*
{
    return qobject_cast<QProgressBar*>(m_rendered_widget);
}


auto html::elements::html_progress_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    v8pp::class_<html_progress_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .inherit<mixins::labelable<html_progress_element>>()
            .var("value", &html_progress_element::value)
            .var("max", &html_progress_element::max)
            .var("position", &html_progress_element::position)
            .auto_wrap_objects();
}
