#include "html_title_element.hpp"

#include <dom/helpers/trees.hpp>
#include <dom/helpers/node_internals.hpp>

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_title_element::html_title_element() = default;


auto html::elements::html_title_element::get_text() const
        -> ext::string
{
    // the text of the title node is all the child text content
    return dom::helpers::trees::child_text_content(this);
}


auto html::elements::html_title_element::set_text(
        const ext::string& val)
        -> void
{
    // setting the test replaces all the text content with the new val
    dom::helpers::node_internals::string_replace_all(val, this);
}


auto html::elements::html_title_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_title_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("text", &html_title_element::text)
            .auto_wrap_objects();
}
