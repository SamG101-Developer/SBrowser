#pragma once
#ifndef SBROWSER_HTML_META_ELEMENT_HPP
#define SBROWSER_HTML_META_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_meta_element;}


class html::elements::html_meta_element : public html_element
{
public constructors:
    html_meta_element();

public js_properties:
    ext::html_property<ext::string, _T> name;
    ext::html_property<ext::string, _T> http_equiv;
    ext::html_property<ext::string, _T> content;
    ext::html_property<ext::string, _T> media;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private cpp_properties:
    ext::string m_pragma_set_default_language;

private accessors:
    auto set_http_equiv(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_META_ELEMENT_HPP
