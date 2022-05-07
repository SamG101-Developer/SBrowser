#pragma once
#ifndef SBROWSER_CANVAS_TEXT_HPP
#define SBROWSER_CANVAS_TEXT_HPP

#include <optional>
#include <dom_object.hpp>

namespace html::canvas::mixins {template<typename T> class canvas_text;}
namespace html::canvas::paint {class text_metrics;}


template<typename T>
class html::canvas::mixins::canvas_text : public virtual dom_object
{
public js_methods:
    auto fill_text(const ext::string& text, double x, double y, ext::optional<double> max_width) -> void;
    auto stroke_text(const ext::string& text, double x, double y, ext::optional<double> max_width) -> void;
    new_obj auto measure_text(const ext::string& text) -> paint::text_metrics;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_TEXT_HPP
