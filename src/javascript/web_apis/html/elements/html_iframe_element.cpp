#include "html_iframe_element.hpp"

#include <dom/nodes/document.hpp>
#include <dom/nodes/window.hpp>

#include <html/helpers/document_internals.hpp>
#include <html/helpers/custom_html_elements.hpp>


html::elements::html_iframe_element::html_iframe_element()
        : html_element{}
        , mixins::lazy_loading{}
{
    // set the custom accessors
    content_window.getter   = [this] {return get_content_window();};

    sandbox.setter = [this](auto&& PH1) {set_sandbox(std::forward<decltype(PH1)>(PH1));};

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


auto html::elements::html_iframe_element::get_content_window() const
        -> dom::nodes::window_proxy*
{
    // get the window proxy of the nested browsing context
    return content_document->m_browsing_context->nested_browsing_context->window_proxy;
}


auto html::elements::html_iframe_element::set_sandbox(
        const ext::string& val)
        -> void
{
    // split the items by a space into the keywords, and create a filtered list
    auto items = val.split(' ', -1);
    ext::string_vector filtered;

    // define the allowed sandbox values
    ext::vector<ext::string> allowed_values {
            "allow-downloads",
            "allow-forms",
            "allow-modals",
            "allow-orientation-lock",
            "allow-pointer-lock",
            "allow-popups",
            "allow-popups-to-escape-sandbox",
            "allow-presentation",
            "allow-same-origin",
            "allow-scripts",
            "allow-top-navigation",
            "allow-top-navigation-by-user-activation",
            "allow-top-navigation-to-custom-protocols"
    };

    // check that each item in the val is in the allowed_values list
    items.for_each([allowed_values = std::move(allowed_values), &filtered](const ext::string& item) {
        if (allowed_values.contains(item)) filtered.append(item);
    });

    // set the sandbox value to the result of joining the filtered list
    sandbox << filtered.join();
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
