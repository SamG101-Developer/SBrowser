#pragma once
#ifndef SBROWSER_HTML_BUTTON_ELEMENT_HPP
#define SBROWSER_HTML_BUTTON_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/validatable>

#include <QtWidgets/QPushButton>

namespace html::elements {class html_button_element;}
namespace html::elements {class html_form_element;}


class html::elements::html_button_element
        : public html::elements::html_element
        , public html::mixins::validatable
{
public constructors:
    html_button_element();

public properties:
    ext::html_property<ext::string, _T> form_action;
    ext::html_property<ext::string, _T> form_enctype;
    ext::html_property<ext::string, _T> form_method;
    ext::html_property<ext::string, _T> form_target;

    ext::html_property<ext::string, _T> name;
    ext::html_property<ext::string, _T> type;
    ext::html_property<ext::string, _T> value;

    ext::html_property<bool, _T> form_no_validate;
    ext::html_property<bool, _T> disabled;

    ext::html_property<ext::vector<dom::nodes::node*>*> labels;
    ext::html_property<html::elements::html_form_element*> form;

public internal_methods:
    auto render() const -> QPushButton* override;

protected accessors:
    auto set_type(const ext::string& val) -> void;
};


#endif //SBROWSER_HTML_BUTTON_ELEMENT_HPP
