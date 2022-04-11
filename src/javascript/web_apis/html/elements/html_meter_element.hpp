#pragma once
#ifndef SBROWSER_HTML_METER_ELEMENT_HPP
#define SBROWSER_HTML_METER_ELEMENT_HPP

#include <html/elements/html_element.hpp>
#include <html/mixins/labelable.hpp>

#include <QtWidgets/QProgressBar>

namespace html::elements {class html_meter_element;}


class html::elements::html_meter_element
        : public html_element
        , public mixins::labelable<html_meter_element>
{
public constructors:
    html_meter_element();

public js_properties:
    ext::html_property<double, _T> value;
    ext::html_property<double, _T> min;
    ext::html_property<double, _T> max;
    ext::html_property<double, _T> low;
    ext::html_property<double, _T> high;
    ext::html_property<double, _T> optimum;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_HTML_METER_ELEMENT_HPP
