#pragma once
#ifndef SBROWSER_CANVAS_FILTERS_HPP
#define SBROWSER_CANVAS_FILTERS_HPP

#include <variant>
#include <dom_object.hpp>

namespace html::canvas::mixins {template <typename T> class canvas_filters;}
namespace html::canvas::paint {class canvas_filter;}


template<typename T>
class html::canvas::mixins::canvas_filters : public virtual dom_object
{
public constructors:
    canvas_filters();

public js_properties:
    ext::html_property<std::variant<ext::string, paint::canvas_filter*>, _F> filter;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_CANVAS_FILTERS_HPP
