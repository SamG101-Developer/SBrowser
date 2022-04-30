#include "element_content_editable.hpp"

#include <html/elements/html_element.hpp>
#include <html/helpers/content_editing_internals.hpp>


template <typename T>
html::mixins::element_content_editable<T>::element_content_editable()
{
    // constrain the property values
    content_editable.constrain_values({"true", "false", "inherited"});

    // set the custom accessors
    is_content_editable.getter = [this] {return get_is_content_editable();};
}


template <typename T>
auto html::mixins::element_content_editable<T>::get_is_content_editable() const
        -> bool
{
    return helpers::content_editing_internals::is_editing_host(static_cast<const T*>(this));
}


template <typename T>
auto html::mixins::element_content_editable<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<element_content_editable<T>>{isolate}
            .template inherit<dom_object>()
            .template var("contentEditable", &element_content_editable<T>::content_editable)
            .template var("enterKeyHint", &element_content_editable<T>::enter_key_hint)
            .template var("inputMode", &element_content_editable<T>::input_mode)
            .template var("isContentEditable", &element_content_editable<T>::is_content_editable)
            .auto_wrap_objects();
}


template class html::mixins::element_content_editable<html::elements::html_element>;
