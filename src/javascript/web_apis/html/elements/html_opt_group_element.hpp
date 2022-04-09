#pragma once
#ifndef SBROWSER_HTML_OPT_GROUP_ELEMENT_HPP
#define SBROWSER_HTML_OPT_GROUP_ELEMENT_HPP

#include <html/elements/html_element.hpp>

namespace html::elements {class html_opt_group_element;}


class html::elements::html_opt_group_element : public html_element
{
public constructors:
    html_opt_group_element();

public js_properties:
    ext::html_property<bool, _T> disabled;
    ext::html_property<ext::string, _T> label;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private accessors:
    auto set_disabled(bool val) -> void;
};


#endif //SBROWSER_HTML_OPT_GROUP_ELEMENT_HPP
