#include "hyperlinks.hpp"

#include <dom/helpers/shadows.hpp>
#include <dom/nodes/document.hpp>

#include <html/helpers/document_internals.hpp>
#include <html/elements/html_anchor_element.hpp>
#include <html/elements/html_area_element.hpp>
#include <html/elements/html_element.hpp>
#include <html/mixins/targetable.hpp>


auto html::helpers::hyperlinks::cannot_navigate(
        elements::html_element* element)
        -> bool
{
    // an element can't navigate if the document isn't active, or if the element is an anchor element that isn't
    // connected
    return not helpers::document_internals::is_active_document(element->owner_document)
            or (not dynamic_cast<elements::html_anchor_element*>(element) and not dom::helpers::shadows::is_connected(element));
}


template <typename T>
auto html::helpers::hyperlinks::get_an_elements_target(
        mixins::targetable<T>* element)
        -> ext::string
{
    // if the element has a target, then return the target attribute
    if (not element->target->empty())
        return element->target;

    // otherwise, if there is a HTMLBaseElement in the document, return its target attribute
    else if (auto* base_element = static_cast<T*>(element)->owner_document->child_nodes->template cast_all<elements::html_base_element*>().front())
        return base_element->target;

    // otherwise, there is no target, so return the empty string
    return "";
}



template <typename T>
auto html::helpers::hyperlinks::get_an_elements_noopener(
        mixins::targetable<T>* element,
        const ext::string& target)
        -> bool
{
    // return true if the rel is a noopener or a noreferrer
    if (element->rel->contains("noopener") or element->rel->contains("noreferrer"))
        return true;

    // return true if the rel is not an opener, and the target is '_blank'
    if (not element->rel->contains("opener") and target.new_lowercase() == "_blank")
        return true;

    // otherwise, the element is not a noopener, so return false
    return false;
}


template <typename T>
auto html::helpers::hyperlinks::follow_hyperlink(
        mixins::targetable<T>* element,
        const ext::string& hyperlink_suffix)
        -> void
{
    if (cannot_navigate(element))
        return;

    auto replace = false;
    auto* source = element->owner_document->m_browsing_context;

    ext::string target_attribute_value = multi_cast<elements::html_anchor_element*, elements::html_area_element*>(element)
            ? target_attribute_value = get_an_elements_target(element)
            : "";

    auto noopener = get_an_elements_noopener(element, target_attribute_value);
    auto* target = browsing_context_internals::rules_for_choosing_browsing_context(target_attribute_value, element, noopener);
    if (not target)
        return;

    // TODO : requests
}


template <typename T>
auto html::helpers::hyperlinks::allowed_to_download(
        internal::browsing_context* initiator_browsing_context,
        internal::browsing_context* instantiator_browsing_context)
        -> bool
{
    // return false if the initiator browsing context has the sandboxed flag set
    if (initiator_browsing_context and initiator_browsing_context->sandboxing_flags & browser_context_internals::sandboxing_flags::SANDBOXED_DOWNLOADS)
        return false;

    // return false if the instantiator browsing context has the sandboxed flag set
    if (instantiator_browsing_context and instantiator_browsing_context->sandboxing_flags & browser_context_internals::sandboxing_flags::SANDBOXED_DOWNLOADS)
        return false;

    // TODO : other checks here

    // otherwise, the download is allowed, so return true
    return true;
}


template <typename T>
auto html::helpers::hyperlinks::download_hyperlink(
        mixins::targetable<T>* element,
        const ext::string& hyperlink_suffix)
        -> void
{
    if (cannot_navigate(this))
        return;

    if (not allowed_to_download<T>(element->owner_document->m_browsing_context))
        return;

    // TODO : url parsing
}
