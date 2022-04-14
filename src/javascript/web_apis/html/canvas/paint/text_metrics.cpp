#include "text_metrics.hpp"


auto html::canvas::paint::text_metrics::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<text_metrics>{isolate}
            .inherit<dom_object>()
            .var("width", &text_metrics::width)
            .var("actual_bounding_box_left", &text_metrics::actual_bounding_box_left)
            .var("actual_bounding_box_right", &text_metrics::actual_bounding_box_right)
            .var("font_bounding_box_ascent", &text_metrics::font_bounding_box_ascent)
            .var("font_bounding_box_descent", &text_metrics::font_bounding_box_descent)
            .var("actual_bounding_box_ascent", &text_metrics::actual_bounding_box_ascent)
            .var("actual_bounding_box_descent", &text_metrics::actual_bounding_box_descent)
            .var("em_height_ascent", &text_metrics::em_height_ascent)
            .var("em_height_descent", &text_metrics::em_height_descent)
            .var("hanging_baseline", &text_metrics::hanging_baseline)
            .var("alphabetical_baseline", &text_metrics::alphabetical_baseline)
            .var("ideographic_baseline", &text_metrics::ideographic_baseline)
            .auto_wrap_objects();
}
