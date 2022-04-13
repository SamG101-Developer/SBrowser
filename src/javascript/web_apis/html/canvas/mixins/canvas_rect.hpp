#pragma once
#ifndef SBROWSER_CANVAS_RECT_HPP
#define SBROWSER_CANVAS_RECT_HPP

#include <dom_object.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_rect;}


template<typename T>
class html::canvas::mixins::canvas_rect : public virtual dom_object
{
public js_methods:
    auto clear_rect(double x, double y, double w, double h) -> void;
    auto fill_rect(double x, double y, double w, double h) -> void;
    auto stroke_rect(double x, double y, double w, double h) -> void;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_RECT_HPP
