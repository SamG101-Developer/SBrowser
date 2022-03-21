#pragma once
#ifndef SBROWSER_DOCUMENT_INTERNALS_HPP
#define SBROWSER_DOCUMENT_INTERNALS_HPP

#include <ext/vector.hpp>
#include <ext/string.hpp>


namespace dom::nodes {
    class document;
    class window;
    class window_proxy;
}

namespace html {
    namespace helpers {}
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


class document_internals {

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

    dom::nodes::document* container_document;
    dom::nodes::document* active_document;
    dom::nodes::window* active_window;

    int virtual_browsing_context_group_id;
    ext::string initial_url;
    ext::string opener_origin_at_creation;
};



#endif //SBROWSER_DOCUMENT_INTERNALS_HPP
