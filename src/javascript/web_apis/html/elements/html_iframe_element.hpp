#pragma once
#ifndef SBROWSER_HTML_IFRAME_ELEMENT_HPP
#define SBROWSER_HTML_IFRAME_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/lazy_loading.hpp>

namespace dom::nodes {class window_proxy;}
namespace html::elements {class html_iframe_element;}


class html::elements::html_iframe_element
        : public html_element
        , public mixins::lazy_loading
{
public constructors:
    html_iframe_element();

public js_methods:
    same_obj auto get_svg_document() -> dom::nodes::document*;

public js_properties:
    ext::html_property<ext::string, _T> src;
    ext::html_property<ext::string, _T> srcdoc;
    ext::html_property<ext::string, _T> name;
    ext::html_property<ext::string, _T> allow;
    ext::html_property<ext::string, _T> width;
    ext::html_property<ext::string, _T> height;
    ext::html_property<ext::string, _T> referrer_policy;

    ext::html_property<bool, _T> allow_fullscreen;
    ext::html_property<dom::nodes::document*, _T> content_document;
    ext::html_property<dom::nodes::window_proxy*, _T> content_window;
    ext::html_property<ext::string> sandbox;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private cpp_properties:
    bool m_current_navigation_was_lazy_loaded = false;

private accessors:
    auto get_content_window() const -> dom::nodes::window_proxy*;

    auto set_sandbox(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_IFRAME_ELEMENT_HPP
