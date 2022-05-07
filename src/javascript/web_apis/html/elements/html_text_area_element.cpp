#include "html_text_area_element.hpp"

#include <ext/infinity.hpp>
#include <ext/string_switch.hpp>

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/mutation_observers.hpp>

#include <QtGui/QTextCursor>
#include <QtGui/QTextDocumentFragment>


html::elements::html_text_area_element::html_text_area_element()
{
    // constrain the property values
    wrap.constrain_values({"soft", "hard"});

    // set the custom accessor methods
    selection_start.getter = [this] {return get_selection_start();};
    selection_end.getter = [this] {return get_selection_end();};
    selection_direction.getter = [this] {return get_selection_direction();};

    selection_start.setter = [this](auto&& PH1) {set_selection_start(std::forward<decltype(PH1)>(PH1));};
    selection_end.setter = [this](auto&& PH1) {set_selection_end(std::forward<decltype(PH1)>(PH1));};
    selection_direction.setter = [this](auto&& PH1) {set_selection_direction(std::forward<decltype(PH1)>(PH1));};

    // set the attribute values
    m_raw_value = "";

    // create the widget representation
    auto widget = QPointer<QPlainTextEdit>{};
    m_rendered_widget = widget;

    // attach the qt functions
    read_only.template attach_qt_updater(&QPlainTextEdit::setReadOnly, widget);
}


auto html::elements::html_text_area_element::select()
        -> void
{
    // select all from the beginning to the end of the text
    set_selection_range(0, ext::infinity<ulong>{});
}


auto html::elements::html_text_area_element::set_selection_range(
        ulong start,
        ulong end,
        const ext::string& direction)
        -> void
{
    // get the selection object and save the current selection
    QTextCursor text_selection = qt()->textCursor();
    auto current_selection = text_selection.selection();

    // set the new selection from the start to the end position TODO : direction
    text_selection.setPosition(static_cast<int>(start));
    text_selection.setPosition(static_cast<int>(end), QTextCursor::MoveMode::KeepAnchor);
    text_selection.select(QTextCursor::SelectionType::BlockUnderCursor);

    // check that the direction is valid and update it
    selection_direction = direction;

    // fire a select event if the selection has changed TODO : needs a better way to check changing
    if (text_selection.selection().toPlainText() != current_selection.toPlainText())
        dom::helpers::mutation_observers::queue_element_task(this, [this] {dom::helpers::event_dispatching::fire_event<>("select", this);});
}


auto html::elements::html_text_area_element::set_range_text(
        const ext::string& replacement,
        ext::optional<ulong> start,
        ext::optional<ulong> end,
        const ext::string& selection_mode)
        -> void
{
    // set the dirty flag to true
    m_dirty_flag = true;

    // if the start value isn't given, set it to the minimum of the current selection start or the relevant text length
    auto start_value = start.has_value()
            ? start.value()
            : std::min(ext::property_static_cast<size_t>(selection_start), m_relevant_value.length());

    // if the end value isn't iven, set it to the minimum of the current selection end or the relevant text length
    auto end_value = end.has_value()
            ? end.value()
            : std::min(ext::property_static_cast<size_t>(selection_end), m_relevant_value.length());

    // remove all the text between the start and end value from the relevant text
    if (start_value < end_value)
        m_relevant_value.replace(start_value, end_value - start_value, "");

    // the new length is the length of the replacement text, and the new end is the start value + new_length
    auto new_length = replacement.length();
    auto new_end = start_value + new_length;

    // switch on the selection mode type (from fixed set of values)
    string_switch(selection_mode.c_str())
    {
        // select type case
        string_case("select"):

            // set the selection start and end to the new values, to wrap around the text
            selection_start = start_value;
            selection_end = new_end;

        // start type case
        string_case("start"):

            // set the selection start and end to the start value
            selection_start = start_value;
            selection_end = start_value;

        // end type case
        string_case("end"):

            // set the selection start and to the end value
            selection_start = new_end;
            selection_end = new_end;

        // preserve type case
        string_case("preserve") or not start.has_value() and not start.has_value():

            // the old_length if the end_value - the start_value, and the delta is the difference between this and the
            // new_length
            auto old_length = end_value - start_value;
            auto delta = new_length - old_length;

            // increase the selection start by the delta if it is > the end_value
            if (selection_start > end_value)
                selection_start += delta;

            // otherwise, set it to the start_value
            else if (selection_start > start_value)
                selection_start = start_value;

            // increase the selection end by the delta if it is > the end_value
            if (selection_end > end_value)
                selection_end += delta;

            // otherwise, set it to the end_value
            else if (selection_end > start_value)
                selection_end = end_value;
    }

    // set the selection range with the new selection_start and selection_end
    set_selection_range(selection_start, selection_end);
}


auto html::elements::html_text_area_element::qt() const
        -> QPlainTextEdit*
{
    return qobject_cast<QPlainTextEdit*>(m_rendered_widget);
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
            .var("placeholder", &html_text_area_element::placeholder)
            .var("wrap", &html_text_area_element::wrap)
            .var("default_Value", &html_text_area_element::default_value)
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
