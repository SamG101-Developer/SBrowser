#pragma once
#ifndef SBROWSER_LABELABLE_HPP
#define SBROWSER_LABELABLE_HPP

#include <ext/html_property.hpp>
#include <ext/string.hpp>
#include <dom_object.hpp>

namespace html::mixins {template<typename T> class labelable;}


template <typename T>
class html::mixins::labelable : public virtual dom_object
{
public constructors:
    labelable();

public js_properties:
    ext::html_property<ext::string_vector*, _F> labels;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_LABELABLE_HPP
