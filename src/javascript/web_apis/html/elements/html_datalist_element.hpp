#pragma once
#ifndef SBROWSER_HTML_DATALIST_ELEMENT_HPP
#define SBROWSER_HTML_DATALIST_ELEMENT_HPP

#include <html/elements/html_element.hpp>

#include <QtWidgets/QPushButton>

namespace html::elements {class html_datalist_element;}


class html::elements::html_datalist_element : public html_element
{
public constructors:
    html_datalist_element();

public js_properties:
    ext::html_property<ext::vector<dom::nodes::element*>*> options;

public cpp_methods:
    auto qt() const -> QPushButton* override;
};


#endif //SBROWSER_HTML_DATALIST_ELEMENT_HPP
