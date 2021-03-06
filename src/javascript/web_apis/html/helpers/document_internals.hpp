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

    static auto appropriate_template_contents_owner_document(
            dom::nodes::document* document)
            -> dom::nodes::document*;

    static auto update_visibility_state(
            dom::nodes::document* document,
            const ext::string& visibility_state)
            -> void;

    static auto activation_notification(
            internal::browsing_context* browsing_context)
            -> void;

    static auto discard_document(
            dom::nodes::document* document)
            -> void;

    static auto prompt_to_unload(
            dom::nodes::document* document,
            bool recursive_flag = false)
            -> ext::string;

    static auto unload(
            dom::nodes::document* new_document,
            bool recursive_flag = false,
            internal::document_unload_timing_information* unload_timing_info = nullptr)
            -> void;

    static auto abort_document(
            dom::nodes::document* document)
            -> void;

    static auto stop_loading_document(
            dom::nodes::document* document)
            -> void;
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



#endif //SBROWSER_DOCUMENT_INTERNALS_HPP
