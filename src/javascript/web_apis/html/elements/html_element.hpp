#pragma once
#ifndef SBROWSER_HTML_ELEMENT_HPP
#define SBROWSER_HTML_ELEMENT_HPP

#include <ext/html_property.hpp>
#include <ext/map.hpp>

#include <dom/nodes/element.hpp>
#include <html/mixins/html_or_svg_element.hpp>


namespace html::elements {class html_element;}
namespace html::other {class element_internals;}
namespace html::internal {enum class translation_mode;}


class html::elements::html_element
        : public dom::nodes::element
        , public mixins::html_or_svg_element<html_element>
        // , mixins::element_content_editable
{

public constructors:
    html_element();

public js_methods:
    auto click() -> void;
    new_obj auto attach_internals() -> other::element_internals;

public:
    // html
    ext::html_property<ext::string, _T> title;
    ext::html_property<ext::string, _T> lang;
    ext::html_property<ext::string, _T> translate;
    ext::html_property<ext::string, _T> dir;

    ext::html_property<ext::string> access_key_label;
    ext::html_property<ext::string, _T> access_key;
    ext::html_property<ext::string, _T> autocapitalize;
    ext::html_property<ext::string, _T> inner_text;
    ext::html_property<ext::string, _T> outer_text;

    ext::html_property<bool, _T> hidden;
    ext::html_property<bool, _T> inert;
    ext::html_property<bool, _T> draggable;
    ext::html_property<bool, _T> spellcheck;

    // cssom-view
    ext::html_property<element*> offset_parent;
    ext::html_property<long> offset_top;
    ext::html_property<long> offset_left;
    ext::html_property<long> offset_width;
    ext::html_property<long> offset_height;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected:
    explicit html_element(int) {/* NO_HTML_CONSTRUCTOR */}

private cpp_properties:
    bool m_click_in_progress_flag = false;
    bool m_attached_internals;

private cpp_properties:
    internal::translation_mode m_translation_mode;

private accessors:
    // html
    auto get_inner_text() const -> ext::string;
    auto get_outer_text() const -> ext::string;
    auto get_lang() const -> ext::string;
    auto get_translate() const -> ext::string;

    auto set_inner_text(const ext::string& val) -> void;
    auto set_outer_text(const ext::string& val) -> void;

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
