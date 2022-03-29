#pragma once
#ifndef SBROWSER_HTML_DATALIST_ELEMENT_HPP
#define SBROWSER_HTML_DATALIST_ELEMENT_HPP

#include <html/elements/html_element.hpp>

#include <QtWidgets/QComboBox>

namespace html::elements {class html_datalist_element;}


class html::elements::html_datalist_element : public html_element
{
public constructors:
    html_datalist_element();

public properties:
    ext::html_property<ext::vector<dom::nodes::element*>*> options;

public internal_methods:
    auto render() const -> QComboBox* override;
};


#endif //SBROWSER_HTML_DATALIST_ELEMENT_HPP
