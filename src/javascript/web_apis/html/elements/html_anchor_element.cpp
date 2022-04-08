#include "html_anchor_element.hpp"

#include <dom/events/event.hpp>
#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/trees.hpp>

#include <html/elements/html_image_element.hpp>
#include <html/helpers/hyperlinks.hpp>

#include <QtGui/QCursor>


html::elements::html_anchor_element::html_anchor_element()
        : html_paragraph_element{}
        , mixins::targetable<html_anchor_element>{}
        , mixins::html_hyperlink_element_utils{}
{
    // set the custom accessors
    text.getter = [this] {return get_text();};
    text.setter = [this](auto && PH1) {set_text(std::forward<decltype(PH1)>(PH1));};
}


auto html::elements::html_anchor_element::get_text() const
        -> ext::string
{
    // return the text of all the children nodes (ie the text node children data)
    return dom::helpers::trees::child_text_content(this);
}


auto html::elements::html_anchor_element::set_text(
        const ext::string& val)
        -> void
{
    // set the text by replacing all text node data in the children
    dom::helpers::node_internals::string_replace_all(val, this);
}


auto html::elements::html_anchor_element::activation_behaviour(
        dom::events::event* event)
        -> void
{
    // return if there is no href
    if (href->empty())
        return;

    // create the hyperlink suffix
    ext::string hyperlink_suffix;

    // handle image element map if there is one
    if (auto* image_element = ext::property_dynamic_cast<elements::html_image_element*>(event->target); image_element->is_map)
    {
        // set the x and y to the different in the mouse position to the image position (positive value)
        int x = std::max(0, event->is_trusted ? QCursor::pos().x() - image_element->qt()->x() : 0);
        int y = std::max(0, event->is_trusted ? QCursor::pos().y() - image_element->qt()->y() : 0);

        // the hyperlink suffix is the x and y position with comma separation, after a question mark
        hyperlink_suffix = "?" + std::to_string(x) + "," + std::to_string(y);
    }

    // download or follow the hyperlink depending on the download attribute value
    if (not download->empty())
        helpers::hyperlinks::download_hyperlink(this, hyperlink_suffix);
    else
        helpers::hyperlinks::follow_hyperlink(this, hyperlink_suffix);
}


auto html::elements::html_anchor_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_anchor_element>{isolate}
            .ctor<>()
            .inherit<html_paragraph_element>()
            .inherit<mixins::targetable<html_anchor_element>>()
            .inherit<mixins::html_hyperlink_element_utils>()
            .var("download", &html_anchor_element::download)
            .var("ping", &html_anchor_element::ping)
            .var("hreflang", &html_anchor_element::hreflang)
            .var("type", &html_anchor_element::type)
            .var("text", &html_anchor_element::text)
            .var("referrerPolicy", &html_anchor_element::referrer_policy)
            .auto_wrap_objects();
}
