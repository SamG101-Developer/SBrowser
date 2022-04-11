#pragma once
#ifndef SBROWSER_CANVAS_COMPOSITING_HPP
#define SBROWSER_CANVAS_COMPOSITING_HPP

#include <ext/string.hpp>
#include <dom_object.hpp>

namespace html::canvasing::mixins {template <typename T> class canvas_compositing;}


template<typename T>
class html::canvasing::mixins::canvas_compositing : public virtual dom_object
{
public constructors:
    canvas_compositing();

public js_properties:
    ext::html_property<double, _F> global_alpha;
    ext::html_property<ext::string, _F> global_compositing_operation;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_COMPOSITING_HPP
