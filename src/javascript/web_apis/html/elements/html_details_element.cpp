#include "html_details_element.hpp"

#include <dom/helpers/mutation_observers.hpp>

#include <html/helpers/custom_html_elements.hpp>
#include <html/helpers/html_element_internals.hpp>


html::elements::html_details_element::html_details_element() = default;


auto html::elements::html_details_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_details_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("open", &html_details_element::open)
            .auto_wrap_objects();
}


auto html::elements::html_details_element::set_open(
        bool val)
        -> void
{
    // when the open value changes, queue an element task for the details_notification_task_steps, and set the open
    // attribute
    dom::helpers::mutation_observers::queue_element_task(this, [this] {helpers::html_element_internals::details_notification_task_steps(this);});
    open << val;
}
