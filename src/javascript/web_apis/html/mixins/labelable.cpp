#include "labelable.hpp"


template <typename T>
html::mixins::labelable<T>::labelable()
        : dom_object()
{
    // set the property's values
    labels = new ext::string_vector{};
}


template <typename T>
auto html::mixins::labelable<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<labelable<T>>{isolate}
            .template inherit<dom_object>()
            .template var("labels", &labelable<T>::labels)
            .auto_wrap_objects();
}
