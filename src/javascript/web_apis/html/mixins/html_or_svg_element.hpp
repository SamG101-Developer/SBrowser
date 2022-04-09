#pragma once
#ifndef SBROWSER_HTML_OR_SVG_ELEMENT_HPP
#define SBROWSER_HTML_OR_SVG_ELEMENT_HPP

#include <ext/decorators.hpp>
#include <ext/html_property.hpp>
#include <ext/map.hpp>
#include <ext/string.hpp>

#include <dom_object.hpp>

namespace html::mixins {template <typename T> class html_or_svg_element;}


template <typename T>
class html::mixins::html_or_svg_element : virtual public dom_object
{
public js_methods:
    auto blur() -> void;
    auto focus(const ext::string_any_map& options) -> void;

public js_properties:
    ext::html_property<ext::string_string_map> data_set;
    ext::html_property<ext::string> nonce;
    ext::html_property<bool, _T> autofocus;
    ext::html_property<long, _T> tab_index;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_OR_SVG_ELEMENT_HPP
