#pragma once
#ifndef SBROWSER_HTML_ELEMENT_HPP
#define SBROWSER_HTML_ELEMENT_HPP

#include <dom/nodes/element.hpp>


namespace html {
    namespace elements {class html_element;}
    namespace other {class element_internals;}
}


class html::elements::html_element
        : public dom::nodes::element
        // , mixins::html_or_svg_element
        // , mixins::element_content_editable
{

public: constructors
    html_element();

public: methods
    void click();
    other::element_internals* attach_internals();

public:
    // html
    ext::dom_property<ext::string, _T> title;
    ext::dom_property<ext::string, _T> lang;
    ext::dom_property<ext::string, _T> translate;
    ext::dom_property<ext::string, _T> dir;

    ext::dom_property<ext::string, _F> accessKeyLabel;
    ext::dom_property<ext::string, _T> accessKey;
    ext::dom_property<ext::string, _T> autocapitalize;
    ext::dom_property<ext::string, _T> innerText;
    ext::dom_property<ext::string, _T> outerText;

    ext::dom_property<bool, _T> hidden;
    ext::dom_property<bool, _T> draggable;
    ext::dom_property<bool, _T> spellcheck;

    // cssom-view
    ext::dom_property<element*, _F> offsetParent;
    ext::dom_property<long, _F> offsetTop;
    ext::dom_property<long, _F> offsetLeft;
    ext::dom_property<long, _F> offsetWidth;
    ext::dom_property<long, _F> offsetHeight;

public: internal_methods
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private: accessors
    // html
    auto get_inner_text() const -> ext::string;
    auto get_outer_text() const -> ext::string;

    auto set_inner_text(ext::cstring& val) -> void;
    auto set_outer_text(ext::cstring& val) -> void;

    // cssom-view
    auto get_offset_top() const -> long;
    auto get_offset_left() const -> long;
    auto get_offset_width() const -> long;
    auto get_offset_height() const -> long;

    auto set_offset_top(long val) -> void;
    auto set_offset_left(long val) -> void;
    auto set_offset_width(long val) -> void;
    auto set_offset_height(long val) -> void;
};


#endif //SBROWSER_HTML_ELEMENT_HPP
