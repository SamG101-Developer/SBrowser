#include "document_or_shadow_root.hpp"

#include <dom/nodes/document.hpp>
#include <dom/nodes/shadow_root.hpp>


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
