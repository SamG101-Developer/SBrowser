#pragma once
#ifndef SBROWSER_HTML_LEGEND_ELEMENT_HPP
#define SBROWSER_HTML_LEGEND_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/form_associated.hpp>

namespace html::elements {class html_legend_element;}


class html::elements::html_legend_element
        : public html_element
        , public mixins::form_associated<html_legend_element>
{
public constructors:
    html_legend_element();

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_LEGEND_ELEMENT_HPP
