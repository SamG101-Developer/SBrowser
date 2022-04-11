#pragma once
#ifndef SBROWSER_HTML_PROGRESS_ELEMENT_HPP
#define SBROWSER_HTML_PROGRESS_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/labelable.hpp>

#include <QtWidgets/QProgressBar>

namespace html::elements {class html_progress_element;}


class html::elements::html_progress_element
        : public html_element
        , public mixins::labelable<html_progress_element>
{
public constructors:
    html_progress_element();

public js_properties:
    ext::html_property<double, _T> value;
    ext::html_property<double, _T> max;
    ext::html_property<double, _F> position;

public cpp_methods:
    auto qt() const -> QProgressBar* override;
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_PROGRESS_ELEMENT_HPP
