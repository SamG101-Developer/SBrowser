#include "document_or_shadow_root.hpp"

#include <dom/nodes/document.hpp>
#include <dom/nodes/shadow_root.hpp>


template class dom::mixins::document_or_shadow_root<dom::nodes::document>;
template class dom::mixins::document_or_shadow_root<dom::nodes::shadow_root>;
