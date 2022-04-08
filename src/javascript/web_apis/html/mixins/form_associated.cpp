#include "form_associated.hpp"


template <typename T>
auto html::mixins::form_associated<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<form_associated<T>>{isolate}
            .template inherit<dom_object>()
            .template var("form", &form_associated<T>::form)
            .auto_wrap_objects();
}
