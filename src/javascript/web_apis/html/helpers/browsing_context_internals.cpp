#include "browsing_context_internals.hpp"

#include <javascript/environment/realms.hpp>

#include <dom/nodes/document.hpp>
#include <dom/nodes/element.hpp>
#include <dom/nodes/window.hpp>
#include <dom/nodes/window_proxy.hpp>


auto html::helpers::browsing_context_internals::set_active_document(
        internal::browsing_context* browsing_context,
        dom::nodes::document* document) -> void
{
    auto* window = javascript::realms::relevant_realm(document).global_object;
    document->visibility_state = browsing_context->top_level_browsing_context()->system_visibility_state;
    browsing_context->window_proxy->s_window = window;
    window->document = document;
    // TODO : set flag
}


auto html::helpers::browsing_context_internals::determine_origin(
        internal::browsing_context* browsing_context,
        internal::sandboxing_flags* sandboxing_flags,
        const ext::string& url,
        const ext::string& invocation_origin)
-> ext::string
{
    if (sandboxing_flags->sandboxed_origin_browsing_context_flag)
        return "opaque"; // TODO : change ext::string to origin type (not implemented yet)

    if (url.empty())
        return "opaque";

    if (invocation_origin and url == "about:blank")
        return invocation_origin;

    if (url == "about:srcdoc")
        return browsing_context->container_document()->m_origin;

    return url; // TODO : url_object->origin
}


auto html::helpers::browsing_context_internals::create_new_browsing_context(
        dom::nodes::document* creator,
        dom::nodes::element* embedder,
        const browsing_context_group_t& group)
        -> internal::browsing_context*
{
    auto* browsing_context = new internal::browsing_context{};
    auto unsafe_context_creation_time = performance::helpers::timings::unsafe_shared_current_time();

    if (creator)
    {
        browsing_context->creator_origin = creator->m_origin;
        browsing_context->creator_url = creator->url;
        browsing_context->creator_base_url = creator->base_uri;
        browsing_context->virtual_browsing_context_group_id = creator->m_browsing_context->top_level_browsing_context()->virtual_browsing_context_group_id;
    }

    internal::sandboxing_flags = determine_snadbox_flags(browsing_context, embedder);
    // TODO
}


auto html::helpers::browsing_context_internals::create_new_auxiliary_browsing_context(
        internal::browsing_context* opener)
        -> internal::browsing_context*
{
    auto group = opener->top_level_browsing_context()->group;
    assert(not group.empty());

    auto* browsing_context = create_new_browsing_context(opener->active_document(), nullptr, group);
    browsing_context->opener_browsing_context = opener;
    browsing_context->virtual_browsing_context_group_id = opener->top_level_browsing_context()->virtual_browsing_context_group_id;
    browsing_context->opener_origin_at_creation = opener->active_document()->m_origin;
    group.add(browsing_context);

    // TODO : storage cloning
    return browsing_context;
}


auto html::helpers::browsing_context_internals::create_nested_browsing_context(
        dom::nodes::element* element)
        -> internal::browsing_context*
{
    auto group = element->owner_document->m_browsing_context->top_level_browsing_context()->group;
    auto* browsing_context = create_new_browsing_context(element->owner_document, element, group);
    element->m_nested_browsing_context = browsing_context;

    JS_BLOCK_ENTER
    auto v8_isolate   = v8::Isolate::GetCurrent();
    auto v8_attribute = v8pp::convert<ext::string>::to_v8(v8_isolate, "name");
    auto v8_object    = v8pp::convert<dom::nodes::element*>::to_v8(v8_isolate, element);
    if (v8_object->Has(v8_isolate->GetCurrentContext(), v8_attribute).ToChecked())
    {
        auto v8_name = v8_object->Get(v8_isolate->GetCurrentContext(), v8_attribute).ToLocalChecked();
        browsing_context->name = v8pp::convert<ext::string>::from_v8(v8_isolate, v8_name);
    }
    JS_BLOCK_EXIT
}


auto html::helpers::browsing_context_internals::is_familiar_with(
        internal::browsing_context* context_a,
        internal::browsing_context* context_b)
        -> bool
{
    return context_a->active_document()->m_origin == context_b->active_document()->m_origin
            or context_a->parent_browsing_context and context_a->top_level_browsing_context() == context_b
            or is_auxiliary_browsing_context(context_b) and is_familiar_with(context_a, context_b->opener_browsing_context);
            // TODO or not context_b->is_top_level_context() and
}


auto html::helpers::browsing_context_internals::parse_sandboxing_directive(
        const ext::string& input,
        internal::sandboxing_flags& output)
        -> void
{
    auto tokens = input.split_all(' ');

    output.sandboxed_navigation_browsing_context_flag = true;

    output.sandboxed_auxiliary_navigation_browsing_context_flag = not tokens.contains("allow-popups");

    output.sandboxed_top_level_navigation_without_user_activation_browsing_context_flag = not tokens.contains("allow-top-navigation");

    output.sandboxed_top_level_navigation_with_user_activation_browsing_context_flag = output.sandboxed_top_level_navigation_without_user_activation_browsing_context_flag
            or not tokens.contains("allow-top-navigation-by-user-interaction");

    output.sandboxed_plugins_browsing_context_flag = true;

    output.sandboxed_origin_browsing_context_flag = not tokens.contains("allow-same-origin");

    output.sandboxed_forms_browsing_context_flag = not tokens.contains("allow-forms");

    output.sandboxed_pointer_lock_browsing_context_flag = not tokens.contains("allow-pointer-lock");

    output.sandboxed_scripts_browsing_context_flag = not tokens.contains("allow-scripts");

    output.sandboxed_automatic_features_browsing_context_flag = output.sandboxed_scripts_browsing_context_flag;

    output.sandboxed_document_domain_browsing_context_flag = true;

    output.sandbox_propagates_to_auxiliary_browsing_contexts_flag = not tokens.contains("allow-popups-to-escape-sandbox");

    output.sandboxed_modals_flag = not tokens.contains("allow-modals");

    output.sandboxed_orientation_lock_browsing_context_flag = not tokens.contains("allow-orientation-lock");

    output.sandboxed_presentation_browsing_context_flag = not tokens.contains("allow-presentation");

    output.sandboxed_downloads_browsing_context_flag = not tokens.contains("allow-downloads");

    output.sandboxed_custom_protocols_navigation_browsing_context_flag =
            output.sandboxed_top_level_navigation_without_user_activation_browsing_context_flag
            or not output.sandboxed_auxiliary_navigation_browsing_context_flag
            or not tokens.contains("allow-top-navigation-to-custom-protocols");
}


auto html::helpers::browsing_context_internals::determine_creation_sandboxing_flags(
        internal::browsing_context* browsing_context,
        dom::nodes::element* embedder)
        -> internal::sandboxing_flags&
{
    return not embedder
            ? browsing_context->popup_sandboxing_flags_set
            : embedder->owner_document->m_active_document_flags_set;
}


////


auto html::internal::browsing_context::active_document() const
        -> dom::nodes::document*
{
    return active_window()->document;
}


auto html::internal::browsing_context::content_document() const
        -> dom::nodes::document*
{
    if (not nested_browsing_context)
        return nullptr;

    auto* document = nested_browsing_context->active_document();
    if (document->m_origin != "same-origin" /*and TODO*/)
        return nullptr;

    return document;
}


auto html::internal::browsing_context::active_window() const
        -> dom::nodes::window*
{
    return window_proxy->s_window;
}


auto html::internal::browsing_context::is_top_level_context() const
        -> bool
{
    return not parent_browsing_context;
}

