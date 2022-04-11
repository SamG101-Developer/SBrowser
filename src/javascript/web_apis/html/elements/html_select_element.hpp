#pragma once
#ifndef SBROWSER_HTML_SELECT_ELEMENT_HPP
#define SBROWSER_HTML_SELECT_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/labelable.hpp>
#include <html/mixins/validatable.hpp>
#include <ext/listlike.hpp>

#include <QtWidgets/QComboBox>

namespace html::elements {class html_select_element;}


class html::elements::html_select_element
        : public html_element
        , public mixins::form_associated<html_select_element>
        , public mixins::labelable<html_select_element>
        , public mixins::validatable<html_select_element>
        , public ext::listlike<html_element*>
{
public constructors:
    html_select_element();

public js_properties:
    ext::html_property<ext::string, _T> autocomplete;

    ext::html_property<bool, _T> disabled;
    ext::html_property<bool, _T> multiple; // TODO
    ext::html_property<bool, _T> required;

    ext::html_property<ulong, _T> size;

    ext::html_property<ext::vector<html_element*>*, _F> options;
    ext::html_property<ext::vector<html_element*>*, _F> selected_options;

    ext::html_property<long, _F> selected_index;

public cpp_methods:
    auto qt() const -> QComboBox* override;
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private accessors:
    auto set_size(ulong size) -> void;
};


#endif //SBROWSER_HTML_SELECT_ELEMENT_HPP
