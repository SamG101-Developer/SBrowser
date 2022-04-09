#pragma once
#ifndef SBROWSER_HTML_LINK_ELEMENT_HPP
#define SBROWSER_HTML_LINK_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_link_element;}


class html::elements::html_link_element : public html_element
{
public constructors:
    html_link_element();

public js_properties:
    ext::html_property<ext::string, _T> href;
    ext::html_property<ext::string, _T> cross_origin;
    ext::html_property<ext::string, _T> rel;
    ext::html_property<ext::string, _T> as;
    ext::html_property<ext::string, _T> media;
    ext::html_property<ext::string, _T> integrity;
    ext::html_property<ext::string, _T> hreflang;
    ext::html_property<ext::string, _T> type;
    ext::html_property<ext::string, _T> image_srcset;
    ext::html_property<ext::string, _T> image_sizes;
    ext::html_property<ext::string, _T> referrer_policy;
    ext::html_property<bool, _T> disabled;

    ext::html_property<ext::string_vector*> rel_list;
    ext::html_property<ext::string_vector*> sizes;
    ext::html_property<ext::string_vector*> blocking;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_LINK_ELEMENT_HPP
