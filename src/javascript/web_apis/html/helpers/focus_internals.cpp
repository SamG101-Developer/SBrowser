#include "focus_internals.hpp"

#include <dom/nodes/element.hpp>
#include <dom/nodes/document.hpp>
#include <dom/nodes/shadow_root.hpp>

#include <dom/helpers/event_dispatching.hpp>
#include <dom/helpers/shadows.hpp>
#include <dom/helpers/trees.hpp>

#include <html/elements/html_area_element.hpp>
#include <html/elements/html_dialog_element.hpp>
#include <html/elements/html_image_element.hpp>

#include <html/helpers/document_internals.hpp>


auto html::helpers::focus_internals::currently_focussed_area_of_top_level_browsing_context(
        internal::browsing_context* top_level_browsing_context)
        -> dom::nodes::node*
{
    if (not top_level_browsing_context->system_focus)
        return nullptr;

    auto* candidate = top_level_browsing_context->active_document();
    while (auto* nested_browsing_context = candidate->m_focused_area->m_nested_browsing_context)
        candidate = nested_browsing_context->active_document();

    return candidate->m_focused_area ? candidate->m_focused_area : candidate;
}


auto html::helpers::focus_internals::get_the_focusable_area(
        focus_target_t focus_target,
        const ext::string& focus_trigger)
        -> elements::html_element*
{
    auto* focus_target_element = std::get<elements::html_element*>(focus_target);
    auto* focus_target_context = std::get<internal::browsing_context*>(focus_target);

    if (focus_target_element)
    {

        if (auto* area_element = dynamic_cast<elements::html_area_element*>(focus_target_element))
            ; // TODO

        // TODO : scrolling stuff

        if (auto* document_element = ext::property_dynamic_cast<dom::nodes::document*>(focus_target_element->parent); dom::helpers::trees::is_document_element(focus_target_element))
            ; // TODO : return document's viewport (CSS2)

        if (auto* nested_browsing_context = focus_target_element->m_nested_browsing_context)
            return nested_browsing_context->active_document();

        if (dom::helpers::shadows::is_shadow_host(focus_target_element) and dom::helpers::shadows::shadow_root(focus_target_element)->delegates_focus)
            ; // TODO
    }

    else if (focus_target_context)
        return focus_target_context->active_document();

    return nullptr;
}


auto html::helpers::focus_internals::fire_focus_event(
        const ext::string& e,
        dom::nodes::event_target* t,
        dom::nodes::event_target* r)
        -> void
{
    dom::helpers::event_dispatching::fire_event<ui_events::events::focus_event>(e, t, {{"relatedTarget", r}, {"composed", true}}); // TODO : view attribute in map too
}


auto html::helpers::focus_internals::nearest_ancestor_autofocus_scoping_root_element(
        elements::html_element* element)
        -> elements::html_dialog_element*
{
    if (auto* dialog_element = dynamic_cast<elements::html_dialog_element*>(element))
        return dialog_element;

    else if (auto dialog_ancestors = dom::helpers::trees::ancestors(element).cast_all<elements::html_dialog_element*>(); not dialog_ancestors.empty())
        return dialog_ancestors.back();

    // TODO : default return
}
