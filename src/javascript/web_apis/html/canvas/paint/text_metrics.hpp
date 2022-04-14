#pragma once
#ifndef SBROWSER_TEXT_METRICS_HPP
#define SBROWSER_TEXT_METRICS_HPP

#include <dom_object.hpp>

namespace html::canvas::paint {class text_metrics;}


class html::canvas::paint::text_metrics : public virtual dom_object
{
public js_properties:
    ext::html_property<double, _F> width;
    ext::html_property<double, _F> actual_bounding_box_left;
    ext::html_property<double, _F> actual_bounding_box_right;

    ext::html_property<double, _F> font_bounding_box_ascent;
    ext::html_property<double, _F> font_bounding_box_descent;
    ext::html_property<double, _F> actual_bounding_box_ascent;
    ext::html_property<double, _F> actual_bounding_box_descent;
    ext::html_property<double, _F> em_height_ascent;
    ext::html_property<double, _F> em_height_descent;
    ext::html_property<double, _F> hanging_baseline;
    ext::html_property<double, _F> alphabetical_baseline;
    ext::html_property<double, _F> ideographic_baseline;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_TEXT_METRICS_HPP
