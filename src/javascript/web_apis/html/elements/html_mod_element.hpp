#pragma once
#ifndef SBROWSER_HTML_MOD_ELEMENT_HPP
#define SBROWSER_HTML_MOD_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_mod_element;}


class html::elements::html_mod_element : public html_element
{
public constructors:
    html_mod_element();

public js_properties:
    ext::html_property<ext::string, _T> cite;
    ext::html_property<ext::string, _T> date_time;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_MOD_ELEMENT_HPP
