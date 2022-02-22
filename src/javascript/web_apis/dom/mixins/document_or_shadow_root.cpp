#include "document_or_shadow_root.hpp"

#include <dom/nodes/document.hpp>
#include <dom/nodes/shadow_root.hpp>


template <typename T>
auto dom::mixins::document_or_shadow_root<T>::v8(v8::Isolate* isolate) const -> ext::any
{
    return v8pp::class_<dom::mixins::document_or_shadow_root<dom::nodes::node>>{isolate}
            .var("activeElement", &dom::mixins::document_or_shadow_root<dom::nodes::node>::active_element)
            .var("styleSheets", &dom::mixins::document_or_shadow_root<dom::nodes::node>::style_sheets)
            .var("adoptedStyleSheets", &dom::mixins::document_or_shadow_root<dom::nodes::node>::adopted_style_sheets)
            .auto_wrap_objects();
}


template class dom::mixins::document_or_shadow_root<dom::nodes::document>;
template class dom::mixins::document_or_shadow_root<dom::nodes::shadow_root>;
