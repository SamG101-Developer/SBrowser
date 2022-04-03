#include "html_time_element.hpp"

#include <dom/helpers/trees.hpp>


html::elements::html_time_element::html_time_element() = default;


auto html::elements::html_time_element::get_date_time() const
        -> ext::string
{
    // return the date time attribute if it contains anything, otherwise the child text content of this element
    ext::string date_time_string;
    date_time >> date_time_string;

    return date_time_string.empty() ? dom::helpers::trees::child_text_content(this) : date_time_string;
}


auto html::elements::html_time_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_time_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("dateTime", &html_time_element::date_time)
            .auto_wrap_objects();
}
