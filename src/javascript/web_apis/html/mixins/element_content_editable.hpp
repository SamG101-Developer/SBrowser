#pragma once
#ifndef SBROWSER_ELEMENT_CONTENT_EDITABLE_HPP
#define SBROWSER_ELEMENT_CONTENT_EDITABLE_HPP

#include <dom_object.hpp>

namespace html::mixins {template <typename T> class element_content_editable;}


template <typename T>
class html::mixins::element_content_editable : public virtual dom_object
{
public constructors:
    element_content_editable();

public js_properties:
    ext::html_property<ext::string, _T> content_editable;
    ext::html_property<ext::string, _T> enter_key_hint;
    ext::html_property<ext::string, _T> input_mode;
    ext::html_property<bool, _F> is_content_editable;

public cpp_methods:
    auto v8(v8::Isolate *isolate) const -> ext::any override;

private accessors:
    auto get_is_content_editable() const -> bool;
};


#endif //SBROWSER_ELEMENT_CONTENT_EDITABLE_HPP
