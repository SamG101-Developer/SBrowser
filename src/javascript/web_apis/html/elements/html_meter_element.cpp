#include "html_meter_element.hpp"


html::elements::html_meter_element::html_meter_element()
{
    // constrain the property values
    value.clamp_values(min, max);
    low.clamp_values(min, high);
    optimum.clamp_values(min, max);
    high.clamp_values(low, max);

    // create the widget representation
    auto widget = QPointer<QProgressBar>{};
    m_rendered_widget = widget;

    // attach the qt functions
    value.template attach_qt_updater(&QProgressBar::setValue, widget);
    min.template attach_qt_updater(&QProgressBar::setMinimum, widget);
    max.template attach_qt_updater(&QProgressBar::setMaximum, widget);
}


auto html::elements::html_meter_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_meter_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .inherit<mixins::labelable<html_meter_element>>()
            .var("value", &html_meter_element::value)
            .var("min", &html_meter_element::min)
            .var("max", &html_meter_element::max)
            .var("low", &html_meter_element::low)
            .var("high", &html_meter_element::high)
            .var("optimum", &html_meter_element::optimum)
            .auto_wrap_objects();
}
