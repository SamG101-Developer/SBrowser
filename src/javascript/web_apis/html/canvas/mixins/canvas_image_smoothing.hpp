#pragma once
#ifndef SBROWSER_CANVAS_IMAGE_SMOOTHING_HPP
#define SBROWSER_CANVAS_IMAGE_SMOOTHING_HPP

#include <ext/string.hpp>
#include <dom_object.hpp>

namespace html::canvasing::mixins {template <typename T> class canvas_image_smoothing;}


template<typename T>
class html::canvasing::mixins::canvas_image_smoothing : public virtual dom_object
{
public constructors:
    canvas_image_smoothing();

public js_properties:
    ext::html_property<bool, _F> image_smoothing_enabled;
    ext::html_property<ext::string, _F> image_smoothing_quality;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_IMAGE_SMOOTHING_HPP
