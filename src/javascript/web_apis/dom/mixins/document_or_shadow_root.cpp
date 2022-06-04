#include "document_or_shadow_root.hpp"

#include <dom/helpers/shadows.hpp>
#include <dom/helpers/trees.hpp>

#include <dom/nodes/element.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/shadow_root.hpp>


template <typename T>
dom::mixins::document_or_shadow_root<T>::document_or_shadow_root()
{
    // set the custom accessor methods
    bind_get(active_element, get_active_element);

    active_element._meta.getter = std::bind(&document_or_shadow_root<T>::get_active_element, this);
}


template <typename T>
auto dom::mixins::document_or_shadow_root<T>::get_active_element() const
        -> smart_pointer<nodes::element>
{
    nodes::element* candidate = nullptr; // TODO : from html::helpers::focus_internals
    dom::helpers::shadows::retarget(candidate, static_cast<const T*>(this));

    if (dom::helpers::trees::root(candidate) != static_cast<const T*>(this))
        return smart_pointer<nodes::element>(nullptr);

    if (not dynamic_cast<nodes::document*>(candidate))
        return smart_pointer(candidate);

    if (nodes::element* body = dynamic_cast<nodes::document*>(candidate)->body)
        return smart_pointer(body);

    if (nodes::element* document_element = dynamic_cast<nodes::document*>(candidate)->document_element)
        return smart_pointer(document_element);

    return smart_pointer<nodes::element>(nullptr);
}


template <typename T>
auto dom::mixins::document_or_shadow_root<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<document_or_shadow_root<T>>{isolate}
            .template inherit<dom_object>()
            .template var("activeElement", &document_or_shadow_root<T>::active_element)
            .template var("styleSheets", &document_or_shadow_root<T>::style_sheets)
            .template var("adoptedStyleSheets", &document_or_shadow_root<T>::adopted_style_sheets)
            .auto_wrap_objects();
}


template class dom::mixins::document_or_shadow_root<dom::nodes::document>;
template class dom::mixins::document_or_shadow_root<dom::nodes::shadow_root>;
