#pragma once
#ifndef SBROWSER_HTML_DIALOG_ELEMENT_HPP
#define SBROWSER_HTML_DIALOG_ELEMENT_HPP

#include <html/elements/html_element.hpp>

#include <QtWidgets/QDialog>

namespace html::elements {class html_dialog_element;}


class html::elements::html_dialog_element : public html_element
{
public constructors:
    html_dialog_element();

public methods:
    auto show() -> void;
    auto show_modal() -> void;
    auto close(const ext::string& result = "") -> void;

public properties:
    ext::html_property<bool, _T> open;
    ext::html_property<ext::string> return_value;

public internal_methods:
    auto render() const -> QDialog* override;
    auto v8(v8::Isolate *isolate) const -> ext::any override

protected internal_properties:
    dom::nodes::element* m_previously_focused_element;
    bool m_modal_flag;
};


#endif //SBROWSER_HTML_DIALOG_ELEMENT_HPP
