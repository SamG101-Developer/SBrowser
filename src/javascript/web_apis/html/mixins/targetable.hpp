#pragma once
#ifndef SBROWSER_TARGETABLE_HPP
#define SBROWSER_TARGETABLE_HPP

#include <ext/html_property.hpp>
#include <ext/string.hpp>
#include <dom_object.hpp>

namespace html::mixins {template <typename T> class targetable;}


template <typename T>
class html::mixins::targetable : public virtual dom_object
{
public constructors:
    targetable();

public js_properties:
    ext::html_property<ext::string, _T> rel;
    ext::html_property<ext::string> target;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;
};


#endif //SBROWSER_TARGETABLE_HPP
