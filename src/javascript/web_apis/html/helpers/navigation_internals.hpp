#pragma once
#ifndef SBROWSER_NAVIGATION_INTERNALS_HPP
#define SBROWSER_NAVIGATION_INTERNALS_HPP

#include <ext/string.hpp>

namespace html::helpers {struct navigation_internals;}
namespace html::internal {struct navigation_params;}

#include <url/url.hpp>

namespace dom::nodes {class document;}

namespace fetch::http {class request;}
namespace fetch::http {class response;}

namespace html::internal {struct browsing_context;}
namespace html::internal {struct environment;}
namespace html::internal {struct policy_container;}
namespace html::internal {struct sandboxing_flags;}


struct html::internal::navigation_params
{
    using algorithm_t = std::function<fetch::http::response*()>;

    ext::string id;
    fetch::http::request* request;
    fetch::http::response* response;
    ext::string origin;

    internal::policy_container& policy_container;
    internal::sandboxing_flags& final_sandboxing_flag_set;
    ext::string cross_origin_opener_policy;
    ext::string coop_enforcement_result;

    internal::environment& reserved_environment;
    internal::browsing_context* browsing_context;
    ext::string history_handling;

    bool has_cross_origin_redirects;
    algorithm_t process_response_and_end_of_body;
    double unsafe_start_time;

    internal::browsing_context* source_browsing_context;
};


struct html::helpers::navigation_internals
{
    static auto navigate(
            internal::browsing_context* browsing_context,
            const ext::string& resource,
            bool exceptions_enabled = false,
            const ext::string& history_handling_behaviour = "default",
            internal::policy_container* history_policy_container = nullptr,
            const ext::string& navigation_type = "other",
            const ext::string& id = "",
            const html::internal::navigation_params::algorithm_t& process_response_end_of_body = [] {return nullptr;})
            -> void;

    static auto process_navigate_fetch(
            const ext::string& id,
            fetch::http::request* request,
            internal::browsing_context* source_browsing_context,
            internal::browsing_context* browsing_context,
            internal::sandboxing_flags* sandbox_flags,
            internal::policy_container* history_policy_container,
            internal::policy_container* initiator_policy_container,
            bool allowed_to_download,
            bool has_transient_activation,
            const ext::string& incumbent_navigation_origin,
            const ext::string& history_handling_behaviour,
            const internal::navigation_params::algorithm_t& process_response_end_of_body,
            int unsafe_navigation_start_time)
            -> void;

    static auto process_navigation_response(
            const ext::string& navigation_type,
            bool allowed_to_download,
            bool has_transient_activation,
            const internal::navigation_params* navigation_params)
            -> void;

    static auto process_navigate_url_scheme(
            const url::url& url_record,
            internal::browsing_context* browsing_context,
            internal::sandboxing_flags* sandbox_flags,
            bool has_transient_activation)
            -> void;

    auto handoff_to_external_software(
            const url::url& url_record,
            internal::browsing_context* browsing_context,
            internal::sandboxing_flags* sandbox_flags,
            bool has_transient_activation)
            -> void;

    auto execute_javascript_url_request(
            fetch::http::request* request,
            internal::browsing_context* browsing_context,
            const ext::string& initiator_origin)
            -> void;

    auto create_and_initialize_document(
            const ext::string& type,
            const ext::string& content_type,
            internal::navigation_params* navigation_params)
            -> dom::nodes::document*;

    auto update_session_history_with_new_page(
            internal::navigation_params* navigation_params,
            dom::nodes::document* new_document)
            -> void;

    auto try_scroll_to_fragment_of_document(
            dom::nodes::document* document)
            -> void;

    auto html_document_to_be_loaded(
            internal::navigation_params* navigation_params)
            -> void;
};


#endif //SBROWSER_NAVIGATION_INTERNALS_HPP
