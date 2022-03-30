#pragma once
#ifndef SBROWSER_DOCUMENT_INTERNALS_HPP
#define SBROWSER_DOCUMENT_INTERNALS_HPP

#include <ext/vector.hpp>
#include <ext/string.hpp>

#include <performance/time/dom_high_res_timestamp.hpp>


namespace dom::nodes {
    class document;
    class element;
    class window;
    class window_proxy;
}

namespace html {
    namespace helpers {struct document_internals;}
    namespace internal {
        struct policy_container;
        struct permissions_policy;
        struct module_map;
        struct cross_origin_opener_policy;
        struct document_load_timing_information;
        struct document_unload_timing_information;
        struct browsing_context;
        struct sandboxing_flags;
    }
}


struct html::helpers::document_internals {
    static auto is_active_document(dom::nodes::document* document) -> bool;
    static auto is_still_on_initial(internal::browsing_context* context) -> bool;
    static auto is_cookie_averse_document(const dom::nodes::document* document) -> bool;
    static auto allows_adding_render_blocking_elements(const dom::nodes::document* document) -> bool;
    static auto is_render_blocked(const dom::nodes::document* document) -> bool;
    static auto block_rendering(dom::nodes::document* document, dom::nodes::element* element) -> void;
    static auto unblock_rendering(dom::nodes::document* document, dom::nodes::element* element) -> void;
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
