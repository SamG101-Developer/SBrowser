#include "content_editing_internals.hpp"

#include <dom/helpers/trees.hpp>
#include <dom/nodes/document.hpp>

#include <html/elements/html_element.hpp>


auto html::helpers::content_editing_internals::is_editing_host(
        const elements::html_element* element)
        -> bool
{
    return element->content_editable == "true" or dynamic_cast<dom::nodes::document*>(dom::helpers::trees::root(element))->m_design_mode_enabled;
}
