#pragma once
#ifndef SBROWSER_BROWSING_CONTEXT_INTERNALS_HPP
#define SBROWSER_BROWSING_CONTEXT_INTERNALS_HPP

namespace html::helpers {struct browsing_context_internals;}
namespace html::internal {struct browsing_context;}
namespace html::internal {struct sandboxing_flags;}
namespace html::internal {struct embedder_policy;}
namespace html::internal {struct policy_container;}
namespace html::internal {struct session_history_entry;}

#include <ext/set.hpp>
#include <ext/string.hpp>

namespace dom::nodes {class document;}
namespace dom::nodes {class element;}
namespace dom::nodes {class window;}
namespace dom::nodes {class window_proxy;}

namespace html::other {class history;}


struct html::helpers::browsing_context_internals
{
    using browsing_context_group_t     = ext::set<internal::browsing_context*>;
    using browsing_context_group_set_t = ext::set<internal::browsing_context*>;

    static auto set_active_document(
            internal::browsing_context* browsing_context,
            dom::nodes::document* document)
            -> void;

    static auto determine_origin(
            internal::browsing_context* browsing_context,
            internal::sandboxing_flags* sandboxing_flags,
            const ext::string& url,
            const ext::string& invocation_origin)
            -> ext::string;

    static auto create_new_browsing_context(
            dom::nodes::document* creator = nullptr,
            dom::nodes::element* embedder = nullptr,
            const browsing_context_group_t& group = {})
            -> internal::browsing_context*;

    static auto create_new_top_level_context()
            -> internal::browsing_context*;

    static auto create_new_auxiliary_browsing_context(
            internal::browsing_context* opener)
            -> internal::browsing_context*;

    static auto create_nested_browsing_context(
            dom::nodes::element* element)
            -> internal::browsing_context*;

    static auto is_auxiliary_browsing_context(
            internal::browsing_context* context)
            -> bool;

    static auto is_familiar_with(
            internal::browsing_context* context_a,
            internal::browsing_context* context_b)
            -> bool;

    static auto is_allowed_to_navigate(
            internal::browsing_context* context_a,
            internal::browsing_context* context_b)
            -> bool;

    static auto create_new_browsing_context_group()
            -> browsing_context_group_t;

    static auto choose_browsing_context(
            const ext::string& name,
            internal::browsing_context* current,
            bool noopener)
            -> std::tuple<internal::browsing_context*, ext::string>;

    static auto is_script_closable(
            internal::browsing_context* context)
            -> bool;

    static auto close_browsing_context(
            internal::browsing_context* context)
            -> void;

    static auto discard_browsing_context(
            internal::browsing_context* context)
            -> void;

    static auto parse_sandboxing_directive(
            const ext::string& input,
            internal::sandboxing_flags& output)
            -> void;

    static auto determine_creation_sandboxing_flags(
            internal::browsing_context* browsing_context,
            dom::nodes::element* embedder = nullptr)
            -> internal::sandboxing_flags&;
};


// TODO : move methods to free functions (is_...)
struct html::internal::browsing_context {
    dom::nodes::window_proxy* window_proxy;

    browsing_context* opener_browsing_context = nullptr;
    browsing_context* nested_browsing_context = nullptr;
    browsing_context* parent_browsing_context = nullptr;

    bool disowned;
    bool system_focus;
    bool is_closing;
    bool is_popup;

    ext::vector<dom::nodes::document*> session_history;

    auto active_document   () const -> dom::nodes::document*;
    auto container_document() const -> dom::nodes::document*;
    auto content_document  () const -> dom::nodes::document*;

    auto active_window() const -> dom::nodes::window*;
    auto top_level_browsing_context() const -> browsing_context*;

    auto is_top_level_context() const -> bool;

    int virtual_browsing_context_group_id = 0;
    ext::string initial_url;
    ext::string opener_origin_at_creation;

    ext::string creator_origin;
    ext::string creator_url;
    ext::string creator_base_url;

    ext::string system_visibility_state;
    ext::string        visibility_state;

    helpers::browsing_context_internals::browsing_context_group_t group;

    ext::string name;
    ext::string cross_origin_isolation_mode = "none";
    sandboxing_flags& popup_sandboxing_flags_set;

    ext::vector<session_history_entry*> joint_session_history;
    session_history_entry* current_entry_joint_session_history;
};


struct html::internal::sandboxing_flags
{
    bool sandboxed_navigation_browsing_context_flag;
    bool sandboxed_auxiliary_navigation_browsing_context_flag;
    bool sandboxed_top_level_navigation_without_user_activation_browsing_context_flag;
    bool sandboxed_top_level_navigation_with_user_activation_browsing_context_flag;
    bool sandboxed_plugins_browsing_context_flag;
    bool sandboxed_origin_browsing_context_flag;
    bool sandboxed_forms_browsing_context_flag;
    bool sandboxed_pointer_lock_browsing_context_flag;
    bool sandboxed_scripts_browsing_context_flag;
    bool sandboxed_automatic_features_browsing_context_flag;
    bool sandboxed_document_domain_browsing_context_flag;
    bool sandbox_propagates_to_auxiliary_browsing_contexts_flag;
    bool sandboxed_modals_flag;
    bool sandboxed_orientation_lock_browsing_context_flag;
    bool sandboxed_presentation_browsing_context_flag;
    bool sandboxed_downloads_browsing_context_flag;
    bool sandboxed_custom_protocols_navigation_browsing_context_flag;
};


struct html::internal::embedder_policy
{
    ext::string value = "unsafe-none";
    ext::string reporting_endpoint = "";
    ext::string report_only_value = "unsafe-none";
    ext::string report_only_reporting_endpoint = "";
};


struct html::internal::policy_container
{
    // TODO : csp list
    embedder_policy* embedder_policy_object = new embedder_policy{};
    ext::string referrer_policy = "default";
};


struct html::internal::session_history_entry
{
    url::url& url_record;
    dom::nodes::document* document;
    ext::string serialized_state;
    policy_container* policy_container_obj;
    ext::string scroll_restoration_mode = "auto";
    ext::string scroll_position_data; // TODO : check type
    ext::string browsing_context_name;
    ext::string persisted_user_state;
};


#endif //SBROWSER_BROWSING_CONTEXT_INTERNALS_HPP
