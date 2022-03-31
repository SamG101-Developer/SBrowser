#pragma once
#ifndef SBROWSER_TARGETABLE_HPP
#define SBROWSER_TARGETABLE_HPP

#include <ext/html_property.hpp>
#include <dom_object.hpp>

namespace html::mixins {template <typename T> class targetable;}


template <typename T>
class html::mixins::targetable : virtual public dom_object
{
public js_properties:
    ext::html_property<ext::string> target;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;
};


#endif //SBROWSER_TARGETABLE_HPP
