#pragma once
#ifndef SBROWSER_HTML_ANCHOR_ELEMENT_HPP
#define SBROWSER_HTML_ANCHOR_ELEMENT_HPP

#include <html/elements/html_paragraph_element.hpp>

namespace html::elements {class html_anchor_element;}


class html::elements::html_anchor_element
        : public html_paragraph_element
        , public mixins::html_hyperlink_element_utils
{
public: constructors
    html_anchor_element();

public: properties
    ext::dom_property<ext::string> target;
    ext::dom_property<ext::string> download;
    ext::dom_property<ext::string> ping;
    ext::dom_property<ext::string> rel;
    ext::dom_property<ext::string> hreflang;
    ext::dom_property<ext::string> type;
    ext::dom_property<ext::string> text;
    ext::dom_property<ext::string> referrerPolicy;
    ext::dom_property<ext::vector<ext::string>*> relList;

private: accessors
    auto get_text() const -> ext::string;
    auto set_text(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_ANCHOR_ELEMENT_HPP
