#include "document_internals.hpp"

#include <dom/nodes/document.hpp>


auto html::helpers::document_internals::is_still_on_initial(
        internal::browsing_context* context)
        -> bool
{
    // document is still initial if there is only 1 element in the session history, and that document is initial
    return context->session_history.length() == 1 and context->session_history.front()->m_is_initial;
}


auto html::helpers::document_internals::is_cookie_averse_document(
        const dom::nodes::document* const document)
        -> bool
{
    // the document is cookie averse of there is no browsing context or the scheme isn't "https"
    return document->m_browsing_context == nullptr or document->url->scheme() != "https";
}


auto html::helpers::document_internals::allows_adding_render_blocking_elements(
        const dom::nodes::document* document)
        -> bool
{
    // the document allows adding render blocking elements if the content type is html and the document doesn't have a
    // body element
    return document->content_type == "text/html" and not document->body;
}


auto html::helpers::document_internals::is_render_blocked(
        const dom::nodes::document* document)
        -> bool
{
    // rendering is blocked is the render blocking elements set isn't empty, or the document allows adding render
    // blocking elements. the render has to have timed out to in order to be render-blocked
    return not document->m_render_blocking_elements.empty() or allows_adding_render_blocking_elements(document); // TODO : timing issue too
}


auto html::helpers::document_internals::block_rendering(
        dom::nodes::document* document,
        dom::nodes::element* element)
        -> void
{
    // add the element to the document's render blocking elements, if the document allows adding render blocking
    // elements
    if (allows_adding_render_blocking_elements(document))
        document->m_render_blocking_elements.add(element);
}


auto html::helpers::document_internals::unblock_rendering(
        dom::nodes::document* document,
        dom::nodes::element* element)
        -> void
{
    // remove the element to the document's render blocking elements
    document->m_render_blocking_elements.remove(element);
}
