#include "html_template_element.hpp"

#include <dom/nodes/document_fragment.hpp>

#include <html/helpers/document_internals.hpp>


html::elements::html_template_element::html_template_element()
{
    // create the widget representation
    m_rendered_widget = nullptr;

    // set the property values
    content = new dom::nodes::document_fragment{};
    content->owner_document = helpers::document_internals::appropriate_template_contents_owner_document(owner_document);
}


auto html::elements::html_template_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .var("content", &html_template_element::content)
            .auto_wrap_objects();
}
