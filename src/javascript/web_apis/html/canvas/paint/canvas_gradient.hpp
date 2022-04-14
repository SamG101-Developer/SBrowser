#pragma once
#ifndef SBROWSER_CANVAS_GRADIENT_HPP
#define SBROWSER_CANVAS_GRADIENT_HPP

#include <dom_object.hpp>

namespace html::canvas::paint {class canvas_gradient;}


class html::canvas::paint::canvas_gradient : public virtual dom_object
{
public js_methods:
    auto add_color_stop(double offset, const ext::string& color) -> void;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_GRADIENT_HPP
