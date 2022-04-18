#include "form_associated.hpp"

#include <QtWidgets/QWidget>


template <typename T>
html::mixins::form_associated<T>::form_associated()
{
    // set the property values
    form << nullptr;

    // attach the qt functions
    disabled.template attach_qt_updater(&QWidget::setDisabled, static_cast<T*>(this));
}


template <typename T>
auto html::mixins::form_associated<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<form_associated<T>>{isolate}
            .template inherit<dom_object>()
            .template var("form", &form_associated<T>::form)
            .template var("name", &form_associated<T>::name)
            .template var("type", &form_associated<T>::type)
            .template var("value", &form_associated<T>::value)
            .template var("disabled", &form_associated<T>::disabled)
            .auto_wrap_objects();
}
