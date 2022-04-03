#pragma once
#ifndef SBROWSER_HTML_ANCHOR_ELEMENT_HPP
#define SBROWSER_HTML_ANCHOR_ELEMENT_HPP

#include <html/elements/html_paragraph_element.hpp>
#include <html/mixins/html_hyperlink_element_utils.hpp>
#include <html/mixins/targetable.hpp>

namespace html::elements {class html_anchor_element;}


class html::elements::html_anchor_element
        : public html_paragraph_element
        , public mixins::targetable<html_anchor_element>
        , public mixins::html_hyperlink_element_utils
{
public constructors:
    html_anchor_element();

public js_properties:
    ext::html_property<ext::string> download;
    ext::html_property<ext::string> ping;
    ext::html_property<ext::string> hreflang;
    ext::html_property<ext::string> type;
    ext::html_property<ext::string> text;
    ext::html_property<ext::string> referrer_policy;
    ext::html_property<ext::string_vector*> relList;

public cpp_methods:
    auto activation_behaviour(dom::events::event* event) -> void override;
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private accessors:
    auto get_text() const -> ext::string;
    auto set_text(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_ANCHOR_ELEMENT_HPP
