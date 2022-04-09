#include "html_text_area_element.hpp"


html::elements::html_text_area_element::html_text_area_element()
{
    // constrain the property values
    wrap.constrain_values({"soft", "hard"});

    // attach the qt functions
    read_only.attach_qt_updater(&QPlainTextEdit::setReadOnly, this);

    // set the attributes
    m_raw_value = "";

    // create the widget representation
    m_rendered_widget = QPointer<QPlainTextEdit>{};
}


auto html::elements::html_text_area_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_text_area_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .inherit<mixins::labelable<html_text_area_element>>()
            .inherit<mixins::form_associated<html_text_area_element>>()
            .inherit<mixins::validatable<html_text_area_element>>()
            .function("select", &html_text_area_element::select)
            .function("set_range_text", &html_text_area_element::set_range_text)
            .function("set_selection_range", &html_text_area_element::set_selection_range)
            .var("autocomplete", &html_text_area_element::autocomplete)
            .var("dir_name", &html_text_area_element::dir_name)
            .var("name", &html_text_area_element::name)
            .var("placeholder", &html_text_area_element::placeholder)
            .var("wrap", &html_text_area_element::wrap)
            .var("default_Value", &html_text_area_element::default_Value)
            .var("type", &html_text_area_element::type)
            .var("value", &html_text_area_element::value)
            .var("read_only", &html_text_area_element::read_only)
            .var("required", &html_text_area_element::required)
            .var("cols", &html_text_area_element::cols)
            .var("rows", &html_text_area_element::rows)
            .var("text_length", &html_text_area_element::text_length)
            .var("max_length", &html_text_area_element::max_length)
            .var("min_length", &html_text_area_element::min_length)
            .var("selection_start", &html_text_area_element::selection_start)
            .var("selection_end", &html_text_area_element::selection_end)
            .var("selection_direction", &html_text_area_element::selection_direction)
            .auto_wrap_objects();
}


auto html::elements::html_text_area_element::qt() const
        -> QPlainTextEdit*
{
    return qobject_cast<QPlainTextEdit*>(m_rendered_widget);
}
