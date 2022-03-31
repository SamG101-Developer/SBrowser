#include "html_iframe_element.hpp"

#include <dom/nodes/document.hpp>

#include <html/helpers/custom_html_elements.hpp>


html::elements::html_iframe_element::html_iframe_element()
        : html_element{}
        , mixins::lazy_loading{}
{
    // attach the qt functions
    width.attach_qt_updater(&QWidget::setFixedWidth, this);
    height.attach_qt_updater(&QWidget::setFixedHeight, this);
}


auto html::elements::html_iframe_element::get_svg_document()
        -> dom::nodes::document*
{
    // return the content document if it a svg+xml document TODO : more complex than this
    return content_document->content_type == "image/svg+xml" ? content_document : nullptr;
}


auto html::elements::html_iframe_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_iframe_element>{isolate}
            .ctor<>()
            .inherit<html_element>()
            .inherit<mixins::lazy_loading>()
            .function("getSVGDocument", &html_iframe_element::get_svg_document)
            .var("src", &html_iframe_element::src)
            .var("srcdoc", &html_iframe_element::srcdoc)
            .var("name", &html_iframe_element::name)
            .var("allow", &html_iframe_element::allow)
            .var("width", &html_iframe_element::width)
            .var("height", &html_iframe_element::height)
            .var("referrer_policy", &html_iframe_element::referrer_policy)
            .var("allow_fullscreen", &html_iframe_element::allow_fullscreen)
            .var("content_document", &html_iframe_element::content_document)
            .var("content_window", &html_iframe_element::content_window)
            .auto_wrap_objects();
}
