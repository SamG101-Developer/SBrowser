#pragma once
#ifndef SBROWSER_HTML_SOURCE_ELEMENT_HPP
#define SBROWSER_HTML_SOURCE_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_source_element;}


class html::elements::html_source_element : public html_element
{
public constructors:
    html_source_element();

public js_properties:
    ext::html_property<ext::string, _T> src;
    ext::html_property<ext::string, _T> type;
    ext::html_property<ext::string, _T> srcset;
    ext::html_property<ext::string, _T> sizes;
    ext::html_property<ext::string, _T> media;
    ext::html_property<ulong, _T> width;
    ext::html_property<ulong, _T> height;

protected cpp_methods:
    auto insertion_steps() -> void override;
    auto removal_steps(dom::nodes::node* old_parent = nullptr) -> void override;
};


#endif //SBROWSER_HTML_SOURCE_ELEMENT_HPP
