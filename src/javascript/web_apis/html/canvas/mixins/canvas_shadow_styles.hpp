#pragma once
#ifndef SBROWSER_CANVAS_SHADOW_STYLES_HPP
#define SBROWSER_CANVAS_SHADOW_STYLES_HPP

#include <ext/string.hpp>
#include <dom_object.hpp>

namespace html::canvas::mixins {template<typename T> class canvas_shadow_styles;}


template <typename T>
class html::canvas::mixins::canvas_shadow_styles : virtual public dom_object
{
public constructors:
    canvas_shadow_styles();

public js_properties:
    ext::html_property<double, _F> shadow_offset_x;
    ext::html_property<double, _F> shadow_offset_y;
    ext::html_property<double, _F> shadow_blur;
    ext::html_property<ext::string, _F> shadow_color;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_SHADOW_STYLES_HPP
