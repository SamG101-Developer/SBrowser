#pragma once
#ifndef SBROWSER_BAR_PROP_HPP
#define SBROWSER_BAR_PROP_HPP

#include <dom_object.hpp>

namespace html::other {class bar_prop;}


class html::other::bar_prop : public virtual dom_object
{
public constructors:
    bar_prop();

public js_properties:
    ext::html_property<bool, _F> visible;

public cpp_methods:
    auto v8(v8::Isolate* isolate) const -> ext::any override;

private accessors:
    auto get_visible() const -> bool;
};


#endif //SBROWSER_BAR_PROP_HPP
