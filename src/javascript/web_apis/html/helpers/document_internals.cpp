#include "document_internals.hpp"

#include <dom/helpers/event_dispatching.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/window.hpp>
#include <dom/nodes/window_proxy.hpp>

#include <html/elements/html_element.hpp>
#include <html/helpers/browsing_context_internals.hpp>

#include <url/url.hpp>


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


auto html::helpers::document_internals::advisory_information(
        dom::nodes::element* element)
        -> ext::string
{
    // return the element title if it exists, the parent's advisory information if there is a parent, otherwise an empty
    // string - in other words, move directly up the tree until the title attribute is set, otherwise an empty string
    auto* html_element = dynamic_cast<elements::html_element*>(element);
    return html_element and html_element->title ? html_element->title : element->parent_element
            ? advisory_information(ext::property_dynamic_cast<elements::html_element*>(element->parent_element))
            : "";
}


auto html::helpers::document_internals::shared_declarative_refresh_steps(
        dom::nodes::document* document,
        ext::string input,
        html::elements::html_meta_element* meta_element)
        -> void
{
    // return if the document will declaratively refresh
    if (document->m_will_declaratively_refresh)
        return;

    // remove spaces from the front of the string and point to the start of the string
    input.ltrim(' ');
    auto position = input.begin();

    // iterate until there is a non-numerical character in the string
    ext::string time_string;
    while (std::isdigit(*position++))
        time_string += *position;

    // return if there are no numerical characters or decimal point
    if (time_string.empty())
        if (*position != '.') return;

    // convert the time string to a double
    auto time = std::stod(time_string);
    auto url_record = document->url;

    // if the position isn't at the end of the string then continue
    if (position != input.end())
    {
        // return if there are certain characters, otherwise increment past whitespace
        if (not ext::vector<char>{';', ',', ' '}.contains(*position))
            return;
        while (*position++ == ' ');

        // increment past certain characters, and the past whitespace
        if (ext::vector<char>{';', ',', ' '}.contains(*position))
            ++position;
        while (*position++ == ' ');
    }

    // if the position isn't at the end of the string then continue
    if (position != input.end())
    {
        auto url_string = input.substring(position, input.end());
        if (*position == 'U' or *position == 'u')
            position++;

        if (*position == 'R' or *position == 'r')
            position++;

        if (*position == 'L' or *position == 'l')
            position++;

        // TODO -> parsing (probably use regex to optimize)
    }

    document->m_will_declaratively_refresh = true;
    // TODO -> more
}


auto html::helpers::document_internals::has_a_stylesheet_that_is_blocking_scripts(
        dom::nodes::document* document)
        -> bool
{
    // the document has a blocking stylesheet if the counter for it > 0, or the container document's counter > 0
    return document->m_script_blocking_stylesheet_counter > 0
            or document->m_browsing_context
                    and document->m_browsing_context->container_document()
                    and document->m_browsing_context->container_document()->m_script_blocking_stylesheet_counter > 0;
}


auto html::helpers::document_internals::allowed_to_use(
        dom::nodes::document* document,
        const ext::string& feature)
        -> bool
{
    // return false if there is no browsing context
    if (document->m_browsing_context == nullptr)
        return false;

    // return false if the browsing context's active document isn't document
    if (document->m_browsing_context->active_document() != document)
        return false;

    // TODO : permission policy API


    // default to returning false
    return false;
}


auto html::helpers::document_internals::update_visibility_state(
        dom::nodes::document* document,
        const ext::string& visibility_state)
        -> void
{
    if (document->visibility_state == visibility_state) return;

    document->visibility_state = visibility_state;
    document->m_behaviour.page_visibility_steps(visibility_state);

    dom::helpers::event_dispatching::fire_event<>("visibilitychange", document, ext::string_any_map{{"bubbles", true}});
}
