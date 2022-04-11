#pragma once
#ifndef SBROWSER_DOCUMENT_INTERNALS_HPP
#define SBROWSER_DOCUMENT_INTERNALS_HPP

#include <ext/vector.hpp>
#include <ext/string.hpp>

#include <performance/time/dom_high_res_timestamp.hpp>


namespace dom::nodes {class document;}
namespace dom::nodes {class element;}
namespace dom::nodes {class window;}
namespace dom::nodes {class window_proxy;}
namespace html::elements {class html_meta_element;}
namespace html::helpers {struct document_internals;}
namespace html::internal {struct policy_container;}
namespace html::internal {struct permissions_policy;}
namespace html::internal {struct module_map;}
namespace html::internal {struct cross_origin_opener_policy;}
namespace html::internal {struct document_load_timing_information;}
namespace html::internal {struct document_unload_timing_information;}
namespace html::internal {struct browsing_context;}
namespace html::internal {struct sandboxing_flags;}


struct html::helpers::document_internals {
    static auto is_active_document(
            dom::nodes::document* document)
            -> bool;

    static auto is_still_on_initial(
            internal::browsing_context* context)
            -> bool;

    static auto is_cookie_averse_document(
            const dom::nodes::document* document)
            -> bool;

    static auto allows_adding_render_blocking_elements(
            const dom::nodes::document* document)
            -> bool;

    static auto is_render_blocked(
            const dom::nodes::document* document)
            -> bool;

    static auto is_render_blocking(
            dom::nodes::element* element)
            -> bool;

    static auto block_rendering(
            dom::nodes::document* document,
            dom::nodes::element* element)
            -> void;

    static auto unblock_rendering(
            dom::nodes::document* document,
            dom::nodes::element* element)
            -> void;

    static auto advisory_information(
            dom::nodes::element* element)
            -> ext::string;

    static auto shared_declarative_refresh_steps(
            dom::nodes::document* document,
            ext::string input,
            html::elements::html_meta_element* meta_element = nullptr)
            -> void;

    static auto has_a_stylesheet_that_is_blocking_scripts(
            dom::nodes::document* document)
            -> bool;

    static auto allowed_to_use(
            dom::nodes::document* document,
            const ext::string& feature) // TODO : type
            -> bool;
};


struct html::internal::document_load_timing_information
{
    performance::time::dom_high_res_timestamp navigation_start_time = 0;
    performance::time::dom_high_res_timestamp dom_interactive_time = 0;
    performance::time::dom_high_res_timestamp dom_content_loaded_event_start_time = 0;
    performance::time::dom_high_res_timestamp dom_content_loaded_event_end_time = 0;
    performance::time::dom_high_res_timestamp dom_complete_time = 0;
    performance::time::dom_high_res_timestamp load_event_start_time = 0;
    performance::time::dom_high_res_timestamp load_event_end_time = 0;
};


struct html::internal::document_unload_timing_information
{
    performance::time::dom_high_res_timestamp unload_event_start_time = 0;
    performance::time::dom_high_res_timestamp unload_event_end_time = 0;
};


struct html::internal::browsing_context {
    dom::nodes::window_proxy* window_proxy;

    browsing_context* opener_browsing_context;
    browsing_context* nested_browsing_context;
    browsing_context* parent_browsing_context;

    bool disowned;
    bool is_closing;
    bool system_focus;

    ext::vector<dom::nodes::document*> session_history;

    auto container_document() const -> dom::nodes::document*;
    auto active_document() const -> dom::nodes::document*;
    auto active_window() const -> dom::nodes::window*;

    int virtual_browsing_context_group_id;
    ext::string initial_url;
    ext::string opener_origin_at_creation;
};



#endif //SBROWSER_DOCUMENT_INTERNALS_HPP
