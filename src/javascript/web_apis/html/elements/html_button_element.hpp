#pragma once
#ifndef SBROWSER_HTML_BUTTON_ELEMENT_HPP
#define SBROWSER_HTML_BUTTON_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/form_associated.hpp>
#include <html/mixins/labelable.hpp>
#include <html/mixins/validatable.hpp>

#include <QtWidgets/QPushButton>

namespace html::elements {class html_button_element;}
namespace html::elements {class html_form_element;}


class html::elements::html_button_element
        : public html::elements::html_element
        , public html::mixins::validatable<html_button_element>
        , public html::mixins::labelable<html_button_element>
        , public html::mixins::form_associated<html_button_element>
{
public constructors:
    html_button_element();

public js_properties:
    ext::html_property<ext::string, _T> form_action;
    ext::html_property<ext::string, _T> form_enctype;
    ext::html_property<ext::string, _T> form_method;
    ext::html_property<ext::string, _T> form_target;

    ext::html_property<bool, _T> form_no_validate;

public cpp_methods:
    auto activation_behaviour(dom::events::event* event) -> void override;
    auto qt() const -> QPushButton* override;
    auto v8(v8::Isolate* isolate) const -> ext::any override;

protected accessors:
    auto set_type(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_BUTTON_ELEMENT_HPP
