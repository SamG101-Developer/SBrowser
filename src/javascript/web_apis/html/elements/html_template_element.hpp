#pragma once
#ifndef SBROWSER_HTML_TEMPLATE_ELEMENT_HPP
#define SBROWSER_HTML_TEMPLATE_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace dom::nodes {class document_fragment;}
namespace html::elements {class html_template_element;}


class html::elements::html_template_element : public html_element
{
public constructors:
    html_template_element();

public js_properties:
    ext::html_property<dom::nodes::document_fragment*, _F> content;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_TEMPLATE_ELEMENT_HPP
