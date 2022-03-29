#include "html_anchor_element.hpp"

#include <dom/helpers/node_internals.hpp>
#include <dom/helpers/trees.hpp>
#include <html/helpers/custom_html_elements.hpp>


html::elements::html_anchor_element::html_anchor_element()
        : html_paragraph_element{}
        , mixins::html_hyperlink_element_utils{}
{
    // set the properties
    relList << new ext::vector<ext::string>{};
    local_name << "a";

    // set the custom accessors
    text.getter = [this] {return get_text();};
    text.setter = [this](auto && PH1) {set_text(std::forward<decltype(PH1)>(PH1));};

    HTML_CONSTRUCTOR
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


auto html::elements::html_anchor_element::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<html_anchor_element>{isolate}
            .template ctor<>()
            .template inherit<html_paragraph_element>()
//            .template inherit<mixins::html_hyperlink_element_utils>()
            .auto_wrap_objects();
}
