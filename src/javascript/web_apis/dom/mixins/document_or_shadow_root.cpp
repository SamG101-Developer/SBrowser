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
    active_element.getter = [this] {return get_active_element();};
}


template <typename T>
auto dom::mixins::document_or_shadow_root<T>::get_active_element() const
        -> nodes::element*
{
    nodes::element* candidate = nullptr; // TODO : from html::helpers::focus_internals
    dom::helpers::shadows::retarget(candidate, static_cast<const T*>(this));

    if (dom::helpers::trees::root(candidate) != static_cast<const T*>(this))
        return nullptr;

    if (not dynamic_cast<nodes::document*>(candidate))
        return candidate;

    if (nodes::element* body = dynamic_cast<nodes::document*>(candidate)->body)
        return body;

    if (nodes::element* document_element = dynamic_cast<nodes::document*>(candidate)->document_element)
        return document_element;

    return nullptr;
}


template <typename T>
auto dom::mixins::document_or_shadow_root<T>::v8(
        v8::Isolate* isolate) const
        -> ext::any
{
    return v8pp::class_<document_or_shadow_root<T>>{isolate}
            .inherit<dom_object>()
            .var("activeElement", &document_or_shadow_root<T>::active_element)
            .var("styleSheets", &document_or_shadow_root<T>::style_sheets)
            .var("adoptedStyleSheets", &document_or_shadow_root<T>::adopted_style_sheets)
            .auto_wrap_objects();
}


template class dom::mixins::document_or_shadow_root<dom::nodes::document>;
template class dom::mixins::document_or_shadow_root<dom::nodes::shadow_root>;
