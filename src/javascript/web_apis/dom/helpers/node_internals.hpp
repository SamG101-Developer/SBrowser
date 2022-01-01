#pragma once
#ifndef SBROWSER_NODE_INTERNALS_HPP
#define SBROWSER_NODE_INTERNALS_HPP

#include <ext/iterables/string.hpp>

namespace dom {
    namespace helpers {struct node_internals;}
    namespace nodes {
        class document;
        class element;
        class node;
        class window;
        class window_proxy;
    }

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

namespace html::elements {class html_element;}


struct dom::helpers::node_internals {
    template <typename T> static dom::nodes::node* clone(T* node, nodes::document* document = nullptr, bool deep = false) requires std::is_base_of_v<T, nodes::node>;
    static ext::string locate_a_namespace_prefix(nodes::element* element, ext::cstring& namespace_);
    static ext::string locate_a_namespace(nodes::node* node, ext::cstring& prefix);
    static ext::vector<nodes::element*> list_of_elements_with_qualified_name(nodes::node* descendant_element, ext::cstring& qualified_name);
    static ext::vector<nodes::element*> list_of_elements_with_namespace_and_local_name(nodes::node* node, ext::cstring& namespace_, ext::cstring& local_name);
    static ext::vector<nodes::element*> list_of_elements_with_class_names(nodes::node* node, ext::cstring& class_names);
    static bool equals(nodes::node* node_1, nodes::node* node_2);
    static void adopt(nodes::node* node, nodes::document* document);

    static void string_replace_all(ext::cstring& string, nodes::node* parent);
    static bool is_document_available_to_user(nodes::document* document, ext::cstring&);
    static bool is_document_fully_active(nodes::document* document); // TODO name [-1]
    static bool is_html(nodes::element* element);

    static bool has_active_parser(nodes::document* document);
    static ext::string advisory_information(html::elements::html_element* element);

    static bool document_has_style_sheets_blocking_scripts(nodes::document* document);

    template <typename ...nodes_or_strings> static nodes::node* convert_nodes_into_node(nodes::document* document, nodes_or_strings... nodes);
};


struct dom::internal::browsing_context {
    nodes::window_proxy* window_proxy;

    browsing_context* opener_browsing_context;
    browsing_context* nested_browsing_context;
    browsing_context* parent_browsing_context;

    bool disowned;
    bool is_closing;
    bool system_focus;

    ext::vector<nodes::document*> session_history;

    nodes::document* container_document;
    nodes::document* active_document;
    nodes::window* active_window;

    int virtual_browsing_context_group_id;
    ext::string initial_url;
    ext::string opener_origin_at_creation;
};


#endif //SBROWSER_NODE_INTERNALS_HPP
