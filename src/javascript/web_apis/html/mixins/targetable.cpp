#include "targetable.hpp"

#include <html/elements/html_anchor_element.hpp>
#include <html/elements/html_area_element.hpp>
#include <html/elements/html_form_element.hpp>


template <typename T>
auto html::mixins::targetable<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<targetable<T>>{isolate}
            .template inherit<dom_object>()
            .template var("rel", &targetable::rel)
            .template var("target", &targetable::target)
            .auto_wrap_objects();
}


template class html::mixins::targetable<html::elements::html_anchor_element>;
template class html::mixins::targetable<html::elements::html_area_element>;
template class html::mixins::targetable<html::elements::html_form_element>;
